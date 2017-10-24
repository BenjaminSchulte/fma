export default class Node {
  constructor() {
    this.location = null;
  }

  setLocation(location) {
    this.location = location;
    return this;
  }
}
