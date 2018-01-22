import NestedExpressionNode from './NestedExpressionNode';

export default class ReturnStatement extends NestedExpressionNode {

  static deserialize(s, args) {
    const node = new ReturnStatement();
    node.deserialize(s, args);
    return node;
  }

}
