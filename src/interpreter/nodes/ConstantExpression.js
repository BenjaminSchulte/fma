import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import InterpreterError from '../InterpreterError';

export default class ConstantExpression extends AbstractInterpreter {
  async process() {
    const result = await this.context.process(this.node.parent);
    const object = result.getObject();

    if (!object.hasMember('to_constant')) {
      throw new InterpreterError(`${object.type()} has no member to_constant`);
    }

    return new ValueAccessor(await object.getMember('to_constant').callWithParameters(this.context));
  }
}
