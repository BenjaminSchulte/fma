import ExpressionNode from './ExpressionNode';

export default class BooleanLiteral extends ExpressionNode {
  constructor(value) {
    super();

    this.value = value;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.value
    ];
  }
}
