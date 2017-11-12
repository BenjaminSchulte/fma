import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import InterpreterError from '../InterpreterError';

export default class ConstantExpression extends AbstractInterpreter {
  process() {
    const result = this.context.resolve(this.node.parent);
    var object = result.getObject();

    if (!object.hasMember('to_constant')) {
      if (object.hasMember('to_n')) {
        object = object.getMember('to_n').callWithParameters(this.context);
      } else if (object.hasMember('to_future_number')) {
        object = object.getMember('to_future_number').callWithParameters(this.context);
      }
    }

    if (!object.hasMember('to_constant')) {
      throw new InterpreterError(`${object.type()} has no member to_constant`);
    }

    return new ValueAccessor(this.callWithParameters(object.getMember('to_constant')));
  }
}
