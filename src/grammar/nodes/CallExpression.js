import NestedExpressionNode from './NestedExpressionNode';
import ParameterList from './ParameterList';

export default class CallExpression extends NestedExpressionNode {
  constructor(callback) {
    super(callback);

    this.parameters = new ParameterList();
  }

  setParameters(parameters) {
    this.parameters.setChildren(parameters);
    return this;
  }

  addParameters(parameters) {
    this.parameters.addChildren(parameters);
    return this;
  }

  getParameters() {
    return this.parameters.getChildren();
  }

  setIsRoot() {
    this.parent.setIsRoot();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.parameters.getChildren())
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.parameters.setChildren(s.deserializeList(args[1]));
  }

  static deserialize(s, args) {
    const node = new CallExpression(null);
    node.deserialize(s, args);
    return node;
  }

  asCallExpression(callback) {
    callback(this);
    return this;
  }

  dump() {
    return this.parent.dump() + '(' + this.getParameters().map(par => par ? par.dump() : '???').join(', ') + ')';
  }
}
