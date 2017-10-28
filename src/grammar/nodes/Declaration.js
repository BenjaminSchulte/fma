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
}
