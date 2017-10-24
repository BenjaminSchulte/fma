import ExpressionNode from './ExpressionNode';
import * as ParameterType from './ParameterTypes';

export default class Argument extends ExpressionNode {
  constructor(name) {
    super();

    this.name = name;

    this.defaultValue = null;
    this.type = ParameterType.ARGUMENT;
  }

  setDefaultValue(value) {
    this.defaultValue = value;
    return this;
  }

  setType(type) {
    this.type = type;
    return this;
  }
}
