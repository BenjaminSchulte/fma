import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';

export default class ExpressionStatement extends AbstractInterpreter {
  async process() {

    const result = await this.context.process(this.node.expression);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = await object.callWithParameters(this.context);
    }

    return new ValueAccessor(expressionResult);
  }
}
