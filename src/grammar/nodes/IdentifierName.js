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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.name = s.deserializeString(args[1]);
  }

  static deserialize(s, args) {
    const node = new IdentifierName();
    node.deserialize(s, args);
    return node;
  }
}
