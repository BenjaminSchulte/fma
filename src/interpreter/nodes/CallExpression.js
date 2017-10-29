import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import UndefinedObject from '../../objects/Undefined';
import InterpreterError from '../InterpreterError';

export default class CallExpression extends AbstractInterpreter {
  async process() {

    const target = await this.context.process(this.node.parent);
    var parameters = [];

    for (let parameter of this.node.getParameters()) {
      parameters.push(await this.context.resolve(parameter));
    }

    var result;
    if (!target.getObject().canBeCalled()) {
      if (parameters.length) {
        if (target.object) {
          console.log(target.object);
          throw new InterpreterError('Trying to call an undefined method: ' + target.getTargetType() + '.' + target.getName());
        } else {
          throw new InterpreterError('Trying to call an object of type ' + target.getObjectType());
        }
      }

      result = target.getObject();
    } else {
      result = await target.getObject().callWithParameters(this.context, ...parameters.map(parameter => parameter.getObject()));
    }

    if (!result) {
      result = new UndefinedObject();
    }

    return new ValueAccessor(result);
  }
}
