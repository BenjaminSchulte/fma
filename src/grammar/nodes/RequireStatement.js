import NestedExpressionNode from './NestedExpressionNode';

export default class RequireStatement extends NestedExpressionNode {

  static deserialize(s, args) {
    const node = new RequireStatement();
    node.deserialize(s, args);
    return node;
  }

}
