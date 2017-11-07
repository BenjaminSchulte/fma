import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import InterpreterError from '../InterpreterError';

export default class InlineExpression extends AbstractInterpreter {
  asString() {
    const result = this.context.process(this.node.parent);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = this.callWithParameters(object);
    }

    if (!object.hasMember('to_s')) {
      throw new InterpreterError(`Can not convert ${object.type()} to String`);
    }

    const str = this.callWithParameters(object.getMember('to_s'));

    return str.getMember('__value').getValue();
  }
}
