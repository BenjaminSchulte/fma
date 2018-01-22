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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.value = args[1];
  }

  static deserialize(s, args) {
    const node = new BooleanLiteral(null);
    node.deserialize(s, args);
    return node;
  }
}
