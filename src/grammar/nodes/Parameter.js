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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.name = s.deserialize(args[1]);
    this.value = s.deserialize(args[2]);
    this.parameterType = s.deserializeString(args[3]);
  }

  static deserialize(s, args) {
    const node = new Parameter();
    node.deserialize(s, args);
    return node;
  }

  dump() {
    return this.value.dump();
  }
}
