import NamedObject from './NamedObject';
import ClassInstance from './ClassInstance';
import ArgumentList from '../interpreter/ArgumentList';
import InterpreterError from '../interpreter/InterpreterError';

export default class ClassObject extends NamedObject {
  constructor(name) {
    super(name);

    this.instanceMembers = {};
    this.extended = [];

    this.initializeMembers();
  }

  type() {
    return 'Class';
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

    for (let child of this.extended) {
      if (child.hasInstanceMember(name)) {
        return true;
      }
    }

    return false;
  }

  getInstanceMember(name) {
    if (this.instanceMembers.hasOwnProperty(name)) {
      return this.instanceMembers[name];
    }

    for (let child of this.extended) {
      if (child.hasInstanceMember(name)) {
        return child.getInstanceMember(name);
      }
    }

    return null;
  }

  setInstanceMember(name, value) {
    this.instanceMembers[name] = value;
  }

  onInstance(name, args, callback) {
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = require('./Macro').default;
    const macro = new Macro(name);
    macro.setCallback(callback);
    macro.setArguments(list);

    this.instanceMembers[name] = macro;
  }

  initializeMembers() {
    this.on('new', ['*args', '**kwargs', '&block'], async (context) => {
      const klass = new ClassInstance(this);

      if (this.extended.length == 0) {
        const obj = (await context.getRoot().resolveChild('Object')).getObject();
        this.extendsClass(obj);
      }

      if (klass.hasMember('initialize')) {
        const member = klass.getMember('initialize');
        const callContext = await member.getArguments().buildContextByProxy(context);
        await member.call(callContext);
      }

      return klass;
    })
  }
}
