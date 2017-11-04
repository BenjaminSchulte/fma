import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';

export default class ExpressionStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.expression);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = object.callWithParameters(this.context);
    } else if (object.isUndefined()) {
      this.log("error", "Expression is undefined");
    }

    if (expressionResult.type() === 'Function') {
      const scope = (this.context.getRoot().resolveChild('Compiler')).getObject();
      scope.getMember('current_scope').getMember('on_call_function').callWithParameters(this.context, expressionResult);
    }

    return new ValueAccessor(expressionResult);
  }
}
