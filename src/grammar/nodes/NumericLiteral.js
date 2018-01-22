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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.number = args[1];
  }

  static deserialize(s, args) {
    const node = new NumericLiteral();
    node.deserialize(s, args);
    return node;
  }

  dump() {
    return this.number.toString();
  }
}
