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
}
