import NestedExpressionNode from './NestedExpressionNode';

export default class RangeExpression extends NestedExpressionNode {
  constructor(left, right)  {
    super(left);

    this.right = right;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.right)
    ]
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.right = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new RangeExpression();
    node.deserialize(s, args);
    return node;
  }
}
