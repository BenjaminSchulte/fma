import AbstractInterpreter from './AbstractInterpreter';

export default class ProgramInterpreter extends AbstractInterpreter {
  process() {
    return this.context.processMany(this.node.getChildren());
  }
}
