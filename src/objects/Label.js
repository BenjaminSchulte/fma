import NamedObject from './NamedObject';
import FutureNumber from './FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';

export default class Label extends NamedObject {
  constructor(name) {
    super(name);
  }

  type() {
    return 'Label';
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('to_future_number', [], (self) => {
      return new FutureNumber(new SymbolLocation(self.getSymbolName()));
    })
  }
}
