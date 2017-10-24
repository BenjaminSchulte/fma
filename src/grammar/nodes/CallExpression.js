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
}
