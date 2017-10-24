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
}
