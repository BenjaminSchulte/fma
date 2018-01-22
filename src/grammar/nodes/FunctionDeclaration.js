import BlockDeclaration from './BlockDeclaration';
import StatementList from './StatementList';

export default class FunctionDeclaration extends BlockDeclaration {
  constructor(name) {
    super(name);

    this.decorators = new StatementList();
  }

  setDecorators(decs) {
    this.decorators.setChildren(decs);

    return this;
  }

  getDecorators() {
    return this.decorators.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.decorators.getChildren())
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.decorators.setChildren(s.deserializeList(args[1]));
  }

  static deserialize(s, args) {
    const node = new FunctionDeclaration(null);
    node.deserialize(s, args);
    return node;
  }
}
