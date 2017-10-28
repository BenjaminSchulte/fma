import Node from './Node';

export default class StatementList extends Node {
  constructor() {
    super();

    this.children = [];
  }

  setChildren(children) {
    for (let child of children) {
      this.addChild(child);
    }

    return this;
  }

  addChild(child) {
    if (child === null) {
      return;
    }

    this.children.push(child);

    return this;
  }

  getChildren() {
    return this.children;
  }

  serialize(s) {
    return [super.serialize(s), s.serializeList(this.children)];
  }
}
