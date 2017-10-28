import AbstractInterpreter from './AbstractInterpreter';

export default class IdentifierName extends AbstractInterpreter {
  async asString() {
    return this.node.name;
  }
}
