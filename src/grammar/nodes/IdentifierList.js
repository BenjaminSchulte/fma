import ExpressionList from './ExpressionList';

export default class IdentifierList extends ExpressionList {
  constructor() {
    super();

    this.isRoot = false;
  }

  setIsRoot(root) {
    this.isRoot = root;

    return this;
  }
}
