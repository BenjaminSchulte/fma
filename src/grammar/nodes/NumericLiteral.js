import ExpressionNode from './ExpressionNode';

export default class NumericLiteral extends ExpressionNode {
  constructor(number) {
    super();

    this.number = number;
  }
}
