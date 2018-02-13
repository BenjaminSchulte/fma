import AbstractInterpreter from './AbstractInterpreter';
import FutureNumber from '../../objects/FutureNumber';
import SymbolLocation from '../../linker/calculate/SymbolLocation';

export default class ImportStatement extends AbstractInterpreter {
  process() {

    const text = this.context.getProcessor(this.node.identifier).asString();
    const number = new FutureNumber(new SymbolLocation(text));

    this.context.getObject().setMember(text, number)

  }
}
