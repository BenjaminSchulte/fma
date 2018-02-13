import Node from './Node';

export default class ImportStatement extends Node {
  constructor(identifier) {
    super();

    this.identifier = identifier;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.identifier.serialize(s)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);

    this.identifier = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new ImportStatement();
    node.deserialize(s, args);
    return node;
  }

}
