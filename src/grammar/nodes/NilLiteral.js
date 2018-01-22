import ExpressionNode from './ExpressionNode';

export default class NilLiteral extends ExpressionNode {
  static deserialize(s, args) {
    const node = new NilLiteral();
    node.deserialize(s, args);
    return node;
  }
}
