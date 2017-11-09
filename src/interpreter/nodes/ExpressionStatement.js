import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';

export default class ExpressionStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.expression);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = this.callWithParameters(object);
    } else if (object.isUndefined()) {
      this.log("error", "Expression is undefined");
    }

    if (expressionResult.type() === 'Function') {
      const compiler = (this.context.getRoot().resolveChild('Compiler')).getObject();

      if (compiler.hasMember('current_scope')) {
        const scope = compiler.getMember('current_scope');
        this.callWithParameters(scope.getMember('on_call_function'), expressionResult);
      }
    }

    return new ValueAccessor(expressionResult);
  }
}
