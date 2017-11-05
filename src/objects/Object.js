import PluginUtils from '../plugin/PluginUtils';
import ClassMembers from './ClassMembers';

export default class AbstractObject {
  constructor() {
    this.members = {};
    this.id = AbstractObject.nextId++;
    this.parent = null;
    this.fullName = null;
    this.nameHint = null;
    this.symbolName = null;

    this.functionHooks = [];

    this.klassMembers = null;
  }

  setNameHint(name) {
    this.nameHint = name;
  }

  addFunctionHook(hook) {
    this.functionHooks.push(hook);
  }

  collectFunctionHooks() {
    var list = this.parent ? this.parent.collectFunctionHooks() : [];

    return list.concat(this.functionHooks);
  }

  initializeMembers() {
    if (!this.klassMembers) {
      this.klassMembers = ClassMembers.forClass(this.getKlassName(), this.initializeClassMembers.bind(this));
    }
  }

  getKlassName() {
    return this.type();
  }

  getFullName() {
    if (this.fullName) {
      return this.fullName;
    }

    var prefix = '';

    if (this.parent) {
      prefix = this.parent.getFullName() + '.';

      if (prefix === '.') {
        prefix = '';
      }
    }

    this.fullName = prefix + this.getName();
    return this.fullName;
  }

  getSymbolName() {
    if (this.symbolName === null) {
      this.symbolName = this.getFullName();
    }

    return this.symbolName;
  }

  getName() {
    return this.type() + this.id;
  }

  setParent(parent) {
    this.parent = parent;
  }

  getMembers() {
    return this.members;
  }

  isNil() {
    return false;
  }

  isUndefined() {
    return false;
  }

  hasMember(name) {
    if (this.members.hasOwnProperty(name)) {
      return true;
    }

    this.initializeMembers();
    return this.klassMembers.has(name);
  }

  getMember(name) {
    if (this.members[name]) {
      return this.members[name];
    }

    this.initializeMembers();
    return this.klassMembers.get(name, this);
  }

  setMember(name, object) {
    this.members[name] = object;
  }

  canBeCalled() {
    return false;
  }

  getClassName() {
    return this.type();
  }

  type() {
    return this.constructor.name.replace(/Object$/, '');
  }
/*
  on(name, args, callback) {
    console.log(`DECLARE ${this.type()}.${name}`)

    const ArgumentList = require('../interpreter/ArgumentList').default;
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = require('./Macro').default;
    const macro = new Macro(name);
    macro.setCallback(callback);
    macro.setArguments(list);
    this.setMember(name, macro);
  }*/

  initializeClassMembers(klass) {
    klass.on('nil?', [], (self, context) => {
      return (new PluginUtils(context)).asBoolean(self.isNil());
    })

    klass.on('is_function?', [], (self, context) => {
      const BooleanObject = require('./Boolean').default;
      return new BooleanObject(self.type() === 'Function');
    });

    klass.on('is_a?', ['type'], (self, type, context) => {
      return (new PluginUtils(context)).asBoolean(self.getClassName() == type.getName());
    })

    klass.on('key?', ['key'], (self, key, context) => {
      if (!key.hasMember('to_s')) {
        throw new InterpreterError(`${key.type()} has no member to_s`);
      }

      const BooleanObject = require('./Boolean').default;

      const stringObject = key.getMember('to_s').callWithParameters(context.getContext());
      const str = stringObject.getMember('__value').getValue();
      return new BooleanObject(self.hasMember(str));
    });
  }
}

AbstractObject.nextId = 1;
