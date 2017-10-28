import ExpressionNode from './ExpressionNode';
import * as ParameterType from './ParameterTypes';

export default class Parameter extends ExpressionNode {
  constructor(value) {
    super();

    this.value = value;

    this.name = null;
    this.parameterType = ParameterType.ARGUMENT;
  }

  setName(name) {
    this.name = name;
    return this;
  }

  setType(parameterType) {
    this.parameterType = parameterType;
    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.name),
      s.serialize(this.value),
      s.serializeString(this.parameterType)
    ];
  }
}
