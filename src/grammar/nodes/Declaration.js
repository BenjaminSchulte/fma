import Node from './Node';

export default class Declaration extends Node {
  constructor(name) {
    super();

    this.name = name;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.name)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.name = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new Declaration();
    node.deserialize(s, args);
    return node;
  }
}
