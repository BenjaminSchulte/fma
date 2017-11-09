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
}
