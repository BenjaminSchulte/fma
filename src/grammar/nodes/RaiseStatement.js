import NestedExpressionNode from './NestedExpressionNode';

export default class RaiseStatement extends NestedExpressionNode {
  static deserialize(s, args) {
    const node = new RaiseStatement();
    node.deserialize(s, args);
    return node;
  }
}
