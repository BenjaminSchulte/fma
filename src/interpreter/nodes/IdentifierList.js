import AbstractInterpreter from './AbstractInterpreter';
import InterpreterError from '../InterpreterError';

export default class IdentifierList extends AbstractInterpreter {
  async process() {
    var context = this.context;
    var children = this.node.getChildren().slice();

    if (this.node.isRoot) {
      context = context.getRoot();
    }

    var numChildren = children.length;
    var child;
    for (let i=0; i<numChildren; i++) {
      const identifier = children[i];
      const childName = await this.context.getProcessor(identifier).asString();
      child = await context.resolveChild(childName);

      if (i + 1 < numChildren) {
        throw new InterpreterError("TODO: IdentifierList must autocreate Modules")
      }
    }

    return child;
  }
}
