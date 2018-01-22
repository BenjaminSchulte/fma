import ExpressionNode from './ExpressionNode';
import * as ParameterType from './ParameterTypes';

export default class Argument extends ExpressionNode {
  constructor(name) {
    super();

    this.name = name;

    this.defaultValue = null;
    this.argumentType = ParameterType.ARGUMENT;
  }

  setDefaultValue(value) {
    this.defaultValue = value;
    return this;
  }

  setType(argumentType) {
    this.argumentType = argumentType;
    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.name),
      s.serialize(this.defaultValue),
      s.serializeString(this.argumentType)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.name = s.deserialize(args[1])
    this.defaultValue = s.deserialize(args[2]);
    this.argumentType = s.deserializeString(args[3]);
  }

  static deserialize(s, args) {
    const node = new Argument(null);
    node.deserialize(s, args);
    return node;
  }
}
