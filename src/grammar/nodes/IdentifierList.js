import ExpressionList from './ExpressionList';
import CallExpression from './CallExpression';

export default class IdentifierList extends ExpressionList {
  constructor() {
    super();

    this.isRoot = false;
  }

  setIsRoot(root) {
    this.isRoot = root;

    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.isRoot ? 1 : 0
    ];
  }

  asCallExpression(callback) {
    const expr = new CallExpression(this);
    callback(expr);
    return expr;
  }
}
