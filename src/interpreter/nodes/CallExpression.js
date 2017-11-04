import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import NilObject from '../../objects/Nil';
import InterpreterError from '../InterpreterError';

export default class CallExpression extends AbstractInterpreter {
  process() {

    const target = this.context.process(this.node.parent);
    var parameters = [];

    for (let parameter of this.node.getParameters()) {
      parameters.push(this.context.resolve(parameter));
    }

    var result;
    if (!target.getObject().canBeCalled()) {
      if (parameters.length) {
        if (target.object) {
          console.log(target.object);
          throw new InterpreterError('Trying to call an undefined method: ' + target.getTargetType() + '.' + target.getName());
        } else {
          throw new InterpreterError('Trying to call an object of type ' + target.getObjectType() + ' given by ' + this.node.parent.type());
        }
      }

      result = target.getObject();
    } else {
      result = target.getObject().callWithParameters(this.context, ...parameters.map(parameter => parameter.getObject()));
    }

    if (!result) {
      result = new NilObject();
    }

    return new ValueAccessor(result);
  }
}
