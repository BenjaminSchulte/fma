import Declaration from './Declaration';
import StatementList from './StatementList';

export default class BlockDeclaration extends Declaration {
  constructor(name) {
    super(name);

    this.children = new StatementList();
  }

  setChildren(children) {
    for (let child of children) {
      this.addChild(child);
    }

    return this;
  }

  addChild(child) {
    this.children.addChild(child);
    return this;
  }

  getChildren() {
    return this.children.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.getChildren())
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.setChildren(s.deserializeList(args[1]));
  }

  static deserialize(s, args) {
    const node = new BlockDeclaration(null);
    node.deserialize(s, args);
    return node;
  }
}
