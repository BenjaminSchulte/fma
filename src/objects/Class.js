import NamedObject from './NamedObject';
import ClassInstance from './ClassInstance';
import ArgumentList from '../interpreter/ArgumentList';
import InterpreterError from '../interpreter/InterpreterError';
import VariableTypes from '../interpreter/VariableTypes';
import MacroInstance from './MacroInstance';
import ClassMembers from './ClassMembers';
import FutureNumber from './FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';

export default class ClassObject extends NamedObject {
  constructor(name) {
    super(name);

    this.instanceMembers = {};
    this.extended = [];
    this.memberInitializers = [];
    this.instanceVariables = [];
  }

  initializeMembers() {
    super.initializeMembers();

    if (!this.instance) {
      this.instance = ClassMembers.forClass(this.getKlassName() + ':Instance', this.initializeInstanceMembers.bind(this))
    }
  }

  type() {
    return 'Class';
  }

  getKlassName() {
    return this.getName() + '#' + this.id;
  }

  extendsClass(other) {
    if (other.type() !== 'Class') {
      throw new InterpreterError('Class could not extend object of type ' + other.type());
    }

    this.extended.push(other);
  }

  hasInstanceMember(name) {
    if (this.instanceMembers.hasOwnProperty(name)) {
      return true;
    }

    this.initializeMembers();
    if (this.instance.has(name)) {
      return true;
    }

    for (let child of this.extended) {
      if (child.hasInstanceMember(name)) {
        return true;
      }
    }

    return false;
  }

  getInstanceMember(name, self) {
    if (this.instanceMembers.hasOwnProperty(name)) {
      return this.getInstancedMember(this.instanceMembers[name], self);
    }

    this.initializeMembers();
    if (this.instance.has(name)) {
      return this.instance.get(name, self);
    }

    for (let child of this.extended) {
      if (child.hasInstanceMember(name)) {
        return child.getInstanceMember(name, self);
      }
    }

    return null;
  }

  setInstanceMember(name, value) {
    this.instanceMembers[name] = value;
  }

  getInstancedMember(member, self) {
    if (member.type() !== 'Macro') {
      return member;
    }

    return member.withSelf(self);
  }
/*
  onInstance(name, args, callback) {
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = require('./Macro').default;
    const macro = new Macro(name);
    macro.setCallback(callback);
    macro.setArguments(list);

    this.instanceMembers[name] = macro;
  }
  */

  initializeInstanceVariables(scope, instance, context) {
    for (let parent of this.extended) {
      parent.initializeInstanceVariables(scope, instance, context);
    }

    for (let init of this.memberInitializers) {
      const macro = new MacroInstance(init)
      macro.self = instance;
      macro.callWithParameters(context, scope);
    }
  }

  getOffsetOfInstanceVariable(variable) {
    var currentOffset = 0;

    for (let parent of this.extended) {
      const {found, offset} = parent.getOffsetOfInstanceVariable(variable);

      if (found) {
        return {found: found, offset: currentOffset + offset};
      }

      currentOffset += offset;
    }

    for (let member of this.instanceVariables) {
      if (member.name == variable) {
        return {found: true, offset: currentOffset};
      }

      if (member.klass) {
        currentOffset += member.klass.getSizeOfInstanceVariables();
      } else {
        currentOffset += member.size;
      }
    }

    return {found: false, offset: currentOffset};
  }

  getSizeOfInstanceVariables() {
    var currentOffset = 0;

    for (let member of this.instanceVariables) {
      if (member.klass) {
        currentOffset += member.klass.getSizeOfInstanceVariables();
      } else {
        currentOffset += member.size;
      }
    }

    return currentOffset;
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('new', ['*args', '**kwargs', '&block'], (self, args, kwargs, block, context) => {
      const instance = new ClassInstance(self);

      if (self.extended.length == 0) {
        const obj = (context.getContext().getRoot().resolveChild('Object')).getObject();
        self.extendsClass(obj);
      }

      if (self.memberInitializers.length) {
        const scopeChild = context.getContext().resolveChild('$scope');
        if (scopeChild.isUndefined()) {
          throw new InterpreterError('Can not instanciate a class with RAM instance members, yet');
        }

        const scope = scopeChild.getObject();
        self.initializeInstanceVariables(scope, instance, context.getContext());

      }

      if (instance.hasMember('initialize')) {
        const member = instance.getMember('initialize');
        const callContext = member.getArguments().buildContextByProxy(context.getContext());
        member.call(callContext);
      }

      return instance;
    })

    klass.on('register_variable_offset', ['name', 'as', 'length'], (self, _name, type, _length, context) => {

      const name = context.asString(_name);
      var length;

      if (_length.isNil()) {
        length = 1;
      } else {
        length = context.asNumber(_length);
      }

      if (type.getClassName() === 'String') {
        self.instanceVariables.push({
          name: name,
          size: length * VariableTypes.getSizeOf(context.asString(type))
        });
      } else {
        self.instanceVariables.push({
          name: name,
          klass: type
        });
      }

    });

    klass.on('on_initialize_members', ['&block'], (self, block, context) => {
      self.memberInitializers.push(block.getMacro());
    })

    klass.on('size', [], (self, context) => {
      return context.returnNumber(self.getSizeOfInstanceVariables());
    })

    klass.on('offset_of', ['member'], (self, member, context) => {
      const {found, offset} = self.getOffsetOfInstanceVariable(context.asString(member));

      if (!found) {
        throw new InterpreterError('Could not find class instance member: ' + context.asString(member));
      }

      return context.returnNumber(offset);
    })
  }

  initializeInstanceMembers(klass) {
    klass.on('to_future_number', [], (self, context) => {
      if (!self.memoryScope) {
        throw new InterpreterError('Can not convert an instance which is no memory declaration to future number');
      }

      return new FutureNumber(new SymbolLocation(self.memoryScope.getSymbolName()));
    })
  }
}
