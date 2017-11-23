export default class CallStack {
  constructor() {
    this.stack = [];
  }

  push(node) {
    this.stack.push(node);
  }

  pop() {
    this.stack.pop();
  }

  getLocations() {
    return this.stack.map(item => item.getLocation() ? item.getLocation().toString() : '<unknown>');
  }

  getLocationObjects() {
    return this.stack.filter(item => item.getLocation()).map(item => item.getLocation())
  }
}
