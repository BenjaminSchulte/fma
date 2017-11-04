import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';
import UndefinedObject from '../../objects/Undefined';
import ValueAccessor from '../ValueAccessor';

export default class CalculateExpression extends AbstractInterpreter {
  async process() {

    var left = await this.context.process(this.node.parent);
    var right = (await this.context.resolve(this.node.right)).getObject();
    var operator = this.node.operator;

    var assign = true;
    switch (operator) {
      case '+=': operator='+'; break;
      case '-=': operator='-'; break;
      case '/=': operator='/'; break;
      case '*=': operator='*'; break;
      case '=': operator=''; break;

      default:
        assign = false;
        break;
    }

    var result = right;
    if (operator !== '') {
      var calcLeft = left.getObject();
      if (calcLeft.canBeCalled()) {
        calcLeft = (await calcLeft.callWithParameters(this.context));
      }
      if (!calcLeft.hasMember(operator)) {
        if (calcLeft.hasMember('to_n')) {
          calcLeft = await calcLeft.getMember('to_n').callWithParameters(this.context);
        } else if (calcLeft.hasMember('to_future_number')) {
          calcLeft = await calcLeft.getMember('to_future_number').callWithParameters(this.context);
        }

        if (!calcLeft.hasMember(operator)) {
          throw new InterpreterError('Operator ' + operator + ' not implemented for type: ' + calcLeft.type());
        }
      }

      result = await calcLeft.getMember(operator).callWithParameters(this.context, right);
      if (!result) {
        result = new UndefinedObject();
      }
    }

    if (assign) {
      if (result.type() === 'Undefined') {
        var leftType = left.getTarget();
        var leftName = leftType.name ? leftType.name : leftType.type();
        this.log('warn', `Assigning Undefined to ${leftName}.${left.getName()}`)
      }

      left.define(result);
    }

    return new ValueAccessor(result);
  }
}
