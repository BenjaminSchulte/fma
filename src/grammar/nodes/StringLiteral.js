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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.identifier = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new StringLiteral();
    node.deserialize(s, args);
    return node;
  }

  dump() {
    return JSON.stringify(this.identifier.dump());
  }
}
