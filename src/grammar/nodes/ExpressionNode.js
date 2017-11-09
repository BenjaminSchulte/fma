import Node from './Node';

export default class ExpressionNode extends Node {
  setIsRoot() {
    throw new Error('Can not set ROOT on ' + this.type());
  }

  asCallExpression(callback) {
    throw new Error('Can not make CALL EXPRESSION on ' + this.type());
  }
}
