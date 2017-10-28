import ExpressionNode from './ExpressionNode';

export default class IdentifierName extends ExpressionNode {
  constructor(name) {
    super();

    this.name = name;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeString(this.name)
    ];
  }
}
