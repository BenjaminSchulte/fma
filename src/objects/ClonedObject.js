export default class ClonedObject {
  constructor(other) {
    this.object = other;
  }

  getName() {
    return this.object.getName();
  }

  getFullName() {
    return this.object.getFullName();
  }

  getClassName() {
    return this.object.getClassName();
  }

  getSymbolName() {
    return this.object.getSymbolName();
  }

  type() {
    return this.object.type();
  }

  hasMember(name) {
    return this.object.hasMember(name);
  }

  getMember(name) {
    return this.object.getMember(name);
  }

  setMember(name, value) {
    return this.object.setMember(name, value);
  }

}
