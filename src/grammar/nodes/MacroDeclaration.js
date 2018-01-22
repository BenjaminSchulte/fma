import BlockDeclaration from './BlockDeclaration';
import ArgumentList from './ArgumentList';
import StatementList from './StatementList';

export default class MacroDeclaration extends BlockDeclaration {
  constructor(name) {
    super(name);

    this.arguments = new ArgumentList();
    this.decorators = new StatementList();
    this.isRoot = false;
    this.isDecorator = false;
  }

  setDecorators(decs) {
    this.decorators.setChildren(decs);

    return this;
  }

  getDecorators() {
    return this.decorators.getChildren();
  }

  setIsDecorator(dec) {
    this.isDecorator = dec;
    return this;
  }

  setIsRoot(root) {
    this.isRoot = root;
    return this;
  }

  setArguments(args) {
    this.arguments.setChildren(args);

    return this;
  }

  getArguments() {
    return this.arguments.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.arguments.getChildren()),
      this.isRoot,
      this.isDecorator
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);

    this.arguments.setChildren(s.deserializeList(args[1]));
    this.isRoot = args[2];
    this.isDecorator = args[3];
  }

  static deserialize(s, args) {
    const node = new MacroDeclaration();
    node.deserialize(s, args);
    return node;
  }
}
