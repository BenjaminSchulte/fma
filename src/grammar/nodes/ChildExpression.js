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

  asCallExpression(callback) {
    const expr = new CallExpression(this);
    callback(expr);
    return expr;
  }

  dump() {
    return this.parent.dump() + '.' + this.child.dump();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.child),
      this.isResolved
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.child = s.deserialize(args[1]);
    this.isResolved = args[2];
  }

  static deserialize(s, args) {
    const node = new ChildExpression(null);
    node.deserialize(s, args);
    return node;
  }
}
