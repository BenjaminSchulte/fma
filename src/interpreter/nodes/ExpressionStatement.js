import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';

export default class ExpressionStatement extends AbstractInterpreter {
  async process() {

    const result = await this.context.process(this.node.expression);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = await object.callWithParameters(this.context);
    } else if (object.isUndefined()) {
      this.log("error", "Expression is undefined");
    }

    if (expressionResult.type() === 'Function') {
      const scope = (await this.context.getRoot().resolveChild('Compiler')).getObject();
      await scope.getMember('current_scope').getMember('on_call_function').callWithParameters(this.context, expressionResult);
    }

    return new ValueAccessor(expressionResult);
  }
}
