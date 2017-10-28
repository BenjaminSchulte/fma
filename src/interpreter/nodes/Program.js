import AbstractInterpreter from './AbstractInterpreter';

export default class ProgramInterpreter extends AbstractInterpreter {
  async process() {
    for (let child of this.node.getChildren()) {
      await this.context.process(child);
    }
  }
}
