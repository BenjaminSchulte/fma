import AbstractInterpreter from './AbstractInterpreter';
import ArgumentList from '../ArgumentList';

export default class Argument extends AbstractInterpreter {
  async process() {

    const name = await this.context.getProcessor(this.node.name).asString();

    const list = new ArgumentList();
    list.addArgument(name);

    return list;

  }
}
