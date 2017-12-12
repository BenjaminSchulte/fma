import AbstractObject from './Object';

export default class ReadOnlyBlock extends AbstractObject {
  constructor(target) {
    super();

    this.parent = target;
    this.locked = false;

    this.neverLocked = ['self'];
  }

  hasMember(name) {
    if (this.members.hasOwnProperty(name)) {
      return true;
    }

    return this.parent.hasMember(name);
  }

  getMember(name) {
    if (this.members.hasOwnProperty(name)) {
      return this.members[name];
    }

    return this.parent.getMember(name);
  }

  setMember(name, object) {
    if (this.locked && this.neverLocked.indexOf(name) === -1) {
      return this.parent.setMember(name, object);
    }

    this.members[name] = object;
  }

  lock() {
    this.locked = true;
  }
}
