import ExpressionNode from './ExpressionNode';

export default class NumericLiteral extends ExpressionNode {
  constructor(number) {
    super();

    this.number = number;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.number
    ];
  }

  dump() {
    return this.number.toString();
  }
}
