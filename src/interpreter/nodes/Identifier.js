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
    return this.context.resolveChild(await this.asString());
  }
}
