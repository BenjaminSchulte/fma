import AbstractInterpreter from './AbstractInterpreter';

export default class Identifier extends AbstractInterpreter {
  asString() {
    var text = [];

    for (let child of this.node.getChildren()) {
      text.push(this.context.getProcessor(child).asString());
    }

    return text.join('');
  }

  process() {
    var context = this.context;

    if (this.node.isRoot) {
      context = context.getRoot();
    }

    return context.resolveChild(this.asString());
  }
}
