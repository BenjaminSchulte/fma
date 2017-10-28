export default class AbstractObject {
  constructor() {
    this.members = {};
    this.id = AbstractObject.nextId++;
  }

  hasMember(name) {
    return this.members.hasOwnProperty(name);
  }

  getMember(name) {
    return this.members[name];
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
