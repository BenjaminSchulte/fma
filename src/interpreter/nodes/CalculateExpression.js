import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';
import UndefinedObject from '../../objects/Undefined';
import ValueAccessor from '../ValueAccessor';

export default class CalculateExpression extends AbstractInterpreter {
  async process() {

    var value = await this.context.process(this.node.parent);

    for (let child of this.node.calculations) {
      const right = await this.context.process(child.other);

      value = await this.processCalculation(value, child.operator, right.getObject());
    }

    return value;
  }

  async processCalculation(left, operator, right) {
    if (operator === '=') {
      left.define(right);
      return left;
    }

    left = left.getObject();
    if (!left.hasMember(operator)) {
      throw new InterpreterError('Operator ' + operator + ' not implemented for type: ' + left.type());
    }

    var result = await left.getMember(operator).callWithParameters(this.context, right);
    if (!result) {
      result = new UndefinedObject();
    }

    return new ValueAccessor(result);
  }
}
