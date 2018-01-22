import NestedExpressionNode from './NestedExpressionNode';

export default class ConstantExpression extends NestedExpressionNode {
  static deserialize(s, args) {
    const node = new ConstantExpression();
    node.deserialize(s, args);
    return node;
  }
}
