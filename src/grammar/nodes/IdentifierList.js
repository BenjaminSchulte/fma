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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.isRoot = args[1];
  }

  static deserialize(s, args) {
    const node = new IdentifierList();
    node.deserialize(s, args);
    return node;
  }

  asCallExpression(callback) {
    const expr = new CallExpression(this);
    callback(expr);
    return expr;
  }
}
