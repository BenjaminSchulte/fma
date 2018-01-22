import Node from './Node';

export default class ExpressionStatement extends Node {
  constructor(expression) {
    super();

    this.expression = expression;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.expression)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.expression = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new ExpressionStatement();
    node.deserialize(s, args);
    return node;
  }

  dump() {
    return this.expression.dump();
  }
}
