import ExpressionNode from './ExpressionNode';

export default class StringLiteral extends ExpressionNode {
  constructor(identifier) {
    super();
    this.identifier = identifier;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.identifier)
    ];
  }
}
