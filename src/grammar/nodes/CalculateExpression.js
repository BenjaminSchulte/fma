import NestedExpressionNode from './NestedExpressionNode';

export default class CalculateExpression extends NestedExpressionNode {
  constructor(parent) {
    super(parent);

    this.calculations = [];
  }

  addCalculation(operator, other) {
    this.calculations.push({operator, other});
    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.calculations.map(item => item.operator),
      s.serializeList(this.calculations.map(item => item.other))
    ];
  }
}
