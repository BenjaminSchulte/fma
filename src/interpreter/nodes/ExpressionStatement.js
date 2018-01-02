import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import {Instance as NilObject} from '../../objects/Nil';

export default class ExpressionStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.expression);
    const object = result.getObject();
    var expressionResult = object;

    if (object.canBeCalled()) {
      expressionResult = this.objectOrNil(this.callWithParameters(object));
    } else if (object.isUndefined()) {
      this.log("error", "Expression is undefined");
    }

    if (expressionResult.type() === 'Function') {
      const compiler = (this.context.getRoot().resolveChild('Compiler')).getObject();

      if (compiler.hasMember('current_scope')) {
        const scope = compiler.getMember('current_scope');
        this.callWithParameters(scope.getMember('on_call_function'), expressionResult);
      }

      expressionResult = NilObject;
    }

    return new ValueAccessor(expressionResult);
  }
}
