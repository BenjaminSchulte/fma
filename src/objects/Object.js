import PluginUtils from '../plugin/PluginUtils';

export default class AbstractObject {
  constructor() {
    this.members = {};
    this.id = AbstractObject.nextId++;
    this.parent = null;
    this.fullName = null;

    this.initializeCoreMethods();
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

  getName() {
    return this.type() + this.id;
  }

  setParent(parent) {
    this.parent = parent;
  }

  getMembers() {
    return this.members;
  }

  initializeCoreMethods() {
    this.coreMethods = {};
    this.coreMethods['nil?'] = () => {return PluginUtils.asBoolean(this.isNil())};
  }

  isNil() {
    return false;
  }

  isUndefined() {
    return false;
  }

  hasMember(name) {
    if (this.coreMethods.hasOwnProperty(name)) {
      return true;
    }

    return this.members.hasOwnProperty(name);
  }

  getMember(name) {
    if (this.members[name]) {
      return this.members[name];
    }

    if (this.coreMethods[name]) {
      return this.coreMethods[name]();
    }
  }

  setMember(name, object) {
    this.members[name] = object;
  }

  canBeCalled() {
    return false;
  }

  type() {
    return this.constructor.name.replace(/Object$/, '');
  }

  on(name, args, callback) {
    const ArgumentList = require('../interpreter/ArgumentList').default;
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = require('./Macro').default;
    const macro = new Macro(name);
    macro.setCallback(callback);
    macro.setArguments(list);
    this.setMember(name, macro);
  }
}

AbstractObject.nextId = 1;
