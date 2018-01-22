import Location from '../Location';

export default class Node {
  constructor() {
    this.location = null;
  }

  setLocation(location) {
    this.location = location;
    return this;
  }

  getLocation() {
    return this.location;
  }

  type() {
    return this.constructor.name;
  }

  shouldBeSerialized() {
    return true;
  }

  serialize(s) {
    return s.serializeLocation(this.location);
  }

  dump() {
    return this.type();
  }

  deserialize(s, args) {
    this.location = s.deserializeLocation(args);
  }

  static deserialize(s, args) {
    throw new Error(`Don't know how to deserialize type: ${this.name}`);
  }
}
