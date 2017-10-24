import ExpressionNode from './ExpressionNode';
import * as ParameterType from './ParameterTypes';

export default class Parameter extends ExpressionNode {
  constructor(value) {
    super();

    this.value = value;

    this.name = null;
    this.type = ParameterType.ARGUMENT;
  }

  setName(name) {
    this.name = name;
    return this;
  }

  setType(type) {
    this.type = type;
    return this;
  }
}
