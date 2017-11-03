import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';

export default class RaiseStatement extends AbstractInterpreter {
  async process() {

    const result = await this.context.process(this.node.parent);
    const str = await result.asString(this.context, this.log.bind(this));

    throw new InterpreterError(str);

  }
}
