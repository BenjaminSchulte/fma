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

  asCallExpression(callback) {
    callback(this);
    return this;
  }

  dump() {
    return this.parent.dump() + '(' + this.getParameters().map(par => par ? par.dump() : '???').join(', ') + ')';
  }
}
