import NamedObject from './NamedObject';
import ClassInstance from './ClassInstance';
import ArgumentList from '../interpreter/ArgumentList';
import InterpreterError from '../interpreter/InterpreterError';
import ClassMembers from './ClassMembers';

export default class ClassObject extends NamedObject {
  constructor(name) {
    super(name);

    this.instanceMembers = {};
    this.extended = [];
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

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('new', ['*args', '**kwargs', '&block'], async (self, args, kwargs, block, context) => {
      const instance = new ClassInstance(self);

      if (self.extended.length == 0) {
        const obj = (await context.getContext().getRoot().resolveChild('Object')).getObject();
        self.extendsClass(obj);
      }

      if (instance.hasMember('initialize')) {
        const member = instance.getMember('initialize');
        const callContext = await member.getArguments().buildContextByProxy(context.getContext());
        await member.call(callContext);
      }

      return instance;
    })
  }

  initializeInstanceMembers(klass) {

  }
}
