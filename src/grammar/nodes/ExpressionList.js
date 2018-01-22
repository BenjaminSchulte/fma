import ExpressionNode from './ExpressionNode';
import StatementList from './StatementList';

export default class ExpressionList extends ExpressionNode {
  constructor() {
    super();

    this.children = new StatementList();
  }

  setChildren(children) {
    this.children = new StatementList();
    this.addChildren(children);
    return this;
  }

  addChildren(children) {
    for (let child of children) {
      this.addChild(child);
    }

    return this;
  }

  addChild(child) {
    this.children.addChild(child);
    return this;
  }

  getChildren() {
    return this.children.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.getChildren())
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.setChildren(s.deserializeList(args[1]));
  }

  static deserialize(s, args) {
    const node = new ExpressionList();
    node.deserialize(s, args);
    return node;
  }
}
