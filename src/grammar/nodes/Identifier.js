import ExpressionList from './ExpressionList';
import CallExpression from './CallExpression';

export default class Identifier extends ExpressionList {
  setIsRoot() {
    this.isRoot = true;
  }

  asCallExpression(callback) {
    const expr = new CallExpression(this);
    callback(expr);
    return expr;
  }

  dump() {
    return this.getChildren().map((node) => {
      if (node.type() === 'IdentifierName') {
        return node.name;
      }

      return '\{' + node.dump() + '}';
    }).join('');
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.isRoot
    ]
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.isRoot = args[1];
  }

  static deserialize(s, args) {
    const node = new Identifier();
    node.deserialize(s, args);
    return node;
  }
}
