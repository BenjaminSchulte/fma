import NamedObject from './NamedObject';
import ClassInstance from './ClassInstance';
import ArgumentList from '../interpreter/ArgumentList';
import InterpreterError from '../interpreter/InterpreterError';

export default class ClassObject extends NamedObject {
  constructor(name) {
    super(name);

    this.instanceMembers = {};
    this.instanceMethods = {};
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
    return this.instanceMembers.hasOwnProperty(name) || this.instanceMethods.hasOwnProperty(name);
  }

  getInstanceMember(name) {
    if (!this.instanceMembers.hasOwnProperty(name)) {
      throw new InterpreterError(`Invalid instance member: ${name}`);
    }

    return this.instanceMembers[name];
  }

  setInstanceMember(name, value) {
    console.log("SET MEMBER", this.name + '.' + name)
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

      if (klass.hasMember('initialize')) {
        const member = klass.getMember('initialize');
        const callContext = await member.getArguments().buildContextByProxy(context);
        await member.call(callContext);
      }

      return klass;
    })
  }
}
