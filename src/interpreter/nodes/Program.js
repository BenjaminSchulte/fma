import AbstractInterpreter from './AbstractInterpreter';

export default class ProgramInterpreter extends AbstractInterpreter {
  async process() {
    return await this.context.processMany(this.node.getChildren());
  }
}
