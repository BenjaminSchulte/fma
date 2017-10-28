export default class ValueAccessor {
  constructor(child) {
    this.child = child;
  }

  isUndefined() {
    return this.child.type() === 'Undefined';
  }

  getObject() {
    return this.child;
  }

  getObjectName() {
    if (this.child.name) {
      return this.child.name;
    }

    return this.getObjectType();
  }

  getObjectType() {
    return this.child.type();
  }
}
