import NestedExpressionNode from './NestedExpressionNode';

export default class ChildExpression extends NestedExpressionNode {
  constructor(parent) {
    super(parent);

    this.child = null;
  }

  setChild(child) {
    this.child = child;
    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.child)
    ];
  }
}
