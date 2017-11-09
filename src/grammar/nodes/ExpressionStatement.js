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

  dump() {
    return this.expression.dump();
  }
}
