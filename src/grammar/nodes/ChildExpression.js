import NestedExpressionNode from './NestedExpressionNode';
import CallExpression from './CallExpression';

export default class ChildExpression extends NestedExpressionNode {
  constructor(parent) {
    super(parent);

    this.child = null;
    this.isResolved = false;
  }

  setIsRoot() {
    this.parent.setIsRoot();
  }

  setIsResolved(resolved) {
    this.isResolved = resolved;
    return this;
  }

  setChild(child) {
    this.child = child;
    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.child),
      this.isResolved
    ];
  }

  asCallExpression(callback) {
    const expr = new CallExpression(this);
    callback(expr);
    return expr;
  }

  dump() {
    return this.parent.dump() + '.' + this.child.dump();
  }
}
