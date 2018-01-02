import InterpreterError from '../InterpreterError';
import {Instance as NilObject} from '../../objects/Nil';

export default class AbstractInterpreter {
  constructor(context, node) {
    this.context = context;
    this.comments = null;
    this.node = node;
  }

  objectOrNil(object) {
    if (!object) {
      object = NilObject;
    }
    return object;
  }

  log(type, ...args) {
    var prefix = this.node.getLocation().toString();

    this.context.getInterpreter().log(type, prefix + ": " + args.join(''));
  }

  callWithParameters(object, ...args) {
    return this.context.callWithParameters(this.node, object, ...args)
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

  asString() {
    throw new InterpreterError('asString not implemented');
  }

  preprocess() {
  }

  process() {
    throw new InterpreterError(`process not implemented for ${this.node.type()}`)
  }
}
