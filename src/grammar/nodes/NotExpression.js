import NestedExpressionNode from './NestedExpressionNode';

export default class NotExpression extends NestedExpressionNode {
  static deserialize(s, args) {
    const node = new NotExpression();
    node.deserialize(s, args);
    return node;
  }
}
