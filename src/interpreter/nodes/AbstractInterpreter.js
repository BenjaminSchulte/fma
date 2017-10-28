import InterpreterError from '../InterpreterError';

export default class AbstractInterpreter {
  constructor(context, node) {
    this.context = context;
    this.comments = null;
    this.node = node;
  }

  log(type, ...args) {
    var prefix = this.node.getLocation().toString();

    this.context.getInterpreter().log(type, prefix + ": " + args.join(''));
  }

  setComments(collector) {
    if (!collector.hasComments()) {
      return false;
    }

    const line = this.node.getLocation().getLine();
    if (collector.getLastLine() + 1 >= line) {
      this.comments = collector.getComments();
    }

    return true;
  }

  async asString() {
    throw new InterpreterError('asString not implemented');
  }

  async process() {
    throw new InterpreterError(`process not implemented for ${this.node.type()}`)
  }
}
