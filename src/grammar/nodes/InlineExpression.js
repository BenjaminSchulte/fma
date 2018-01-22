import NestedExpressionNode from './NestedExpressionNode';

export default class InlineExpression extends NestedExpressionNode {
  static deserialize(s, args) {
    const node = new InlineExpression();
    node.deserialize(s, args);
    return node;
  }
}
