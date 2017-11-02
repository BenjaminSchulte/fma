import AbstractInterpreter from './AbstractInterpreter';

export default class Identifier extends AbstractInterpreter {
  async asString() {
    var text = [];

    for (let child of this.node.getChildren()) {
      text.push(await this.context.getProcessor(child).asString());
    }

    return text.join('');
  }

  async process() {
    var context = this.context;

    if (this.node.isRoot) {
      context = context.getRoot();
    }

    return await context.resolveChild(await this.asString());
  }
}
