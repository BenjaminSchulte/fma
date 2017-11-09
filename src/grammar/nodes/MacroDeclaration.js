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
      s.serializeList(this.arguments.getChildren())
    ];
  }
}
