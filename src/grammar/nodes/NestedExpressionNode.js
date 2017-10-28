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
}
