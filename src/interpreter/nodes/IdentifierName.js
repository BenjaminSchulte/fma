import AbstractInterpreter from './AbstractInterpreter';

export default class IdentifierName extends AbstractInterpreter {
  asString() {
    return this.node.name;
  }
}
