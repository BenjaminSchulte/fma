import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';
import ValueAccessor from '../ValueAccessor';
import RangeObject from '../../objects/Range';

export default class RangeExpression extends AbstractInterpreter {
  process() {

    const left = this.context.process(this.node.parent);
    const right = this.context.process(this.node.right);

    if (left.isUndefined() || right.isUndefined()) {
      throw new InterpreterError('Can not create range with Undefined part');
    }

    const leftObject = left.getObject();
    const rightObject = right.getObject();

    if (!leftObject.hasMember('to_n') || !rightObject.hasMember('to_n')) {
      throw new InterpreterError('Operators for range must have to_n method');
    }

    const leftNumber = this.callWithParameters(leftObject.getMember('to_n'))
    const rightNumber = this.callWithParameters(rightObject.getMember('to_n'))
    if (leftNumber.type() !== 'ClassInstance' || rightNumber.type() !== 'ClassInstance' || leftNumber.getClassName() !== 'Number' || rightNumber.getClassName() !== 'Number') {
      throw new InterpreterError('to_n must return Number');
    }

    return new ValueAccessor(new RangeObject(
      leftNumber.getMember('__value').getValue(),
      rightNumber.getMember('__value').getValue()
    ));
  }
}
