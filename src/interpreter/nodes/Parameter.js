import AbstractInterpreter from './AbstractInterpreter';

export default class Parameter extends AbstractInterpreter {
  async process() {

    const value = await this.context.resolve(this.node.value);

    return value;

  }
}
