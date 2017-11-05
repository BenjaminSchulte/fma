import ValueAccessor from './ValueAccessor';

export default class ChildValueAccessor extends ValueAccessor {
  constructor(object, child, childName) {
    super(child);

    this.object = object;
    this.childName = childName;
  }

  getName() {
    return this.childName;
  }

  getTarget() {
    return this.object;
  }

  getTargetName() {
    if (this.object.name) {
      return this.object.name;
    }

    return this.object.type();
  }

  getTargetType() {
    return this.object.type();
  }

  define(object) {
    this.child = object;
    this.object.setMember(this.childName, object);
    object.setParent(this.object);
    object.setNameHint(this.childName);
    return object;
  }
}
