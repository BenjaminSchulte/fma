import ExpressionNode from './ExpressionNode';

export default class NestedExpressionNode extends ExpressionNode {
  constructor(parent) {
    super();

    this.parent = parent;
  }

  setParent(parent) {
    this.parent = parent;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.parent)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.parent = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new NestedExpressionNode();
    node.deserialize(s, args);
    return node;
  }
}
