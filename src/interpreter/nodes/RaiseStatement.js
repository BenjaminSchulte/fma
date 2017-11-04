import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';

export default class RaiseStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.parent);
    const str = result.asString(this.context, this.log.bind(this));

    throw new InterpreterError(str);

  }
}
