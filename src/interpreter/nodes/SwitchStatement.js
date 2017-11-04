import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';

export default class SwitchStatement extends AbstractInterpreter {
  async process() {

    const value = await this.context.resolve(this.node.condition);
    if (value.isUndefined()) {
      throw new InterpreterError('Case argument is undefined');
    }

    const object = value.getObject();
    if (!object.hasMember('==')) {
      throw new InterpreterError(`Object of type ${object.type()} has no == operator`);
    }

    const left = object.getMember('==');
    var otherwise = null;

    for (let child of this.node.getChildren()) {
      switch (child.type()) {
      case 'SwitchCase':
        const right = await this.context.resolve(child.value);
        const result = await left.callWithParameters(this.context, right.getObject());
        if (result.type() !== 'Boolean') {
          throw new InterpreterError(`Operator ${object.type()}.== must return boolean`);
        }

        if (result.isTrue()) {
          // EARLY RETURN!
          return this.context.processMany(child.getChildren());
        }
        break;

      case 'ElseStatement':
        otherwise = child;
        break;

      default:
        throw new InterpreterError('Invalid member in SWITCH/CASE');
      }
    }

    if (otherwise) {
      return await this.context.processMany(otherwise.getChildren());
    }

  }
}
