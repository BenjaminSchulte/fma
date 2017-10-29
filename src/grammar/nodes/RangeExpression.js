import NestedExpressionNode from './NestedExpressionNode';

export default class RangeExpression extends NestedExpressionNode {
  constructor(left, right)  {
    super(left);

    this.right = right;
  }
}
