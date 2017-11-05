import Writer from './writer/Writer';
import LocationHint from './location/LocationHint';

export default class StaticCodeBlock {
  constructor() {
    this.code = new Writer();
    this.locationHint = new LocationHint();
    this.location = null;
  }

  getSize() {
    return this.code.size();
  }

  getLocationHint() {
    return this.locationHint;
  }

  registerInMemory(memory) {
    const child = memory.createChild();
    child.setItemSize(this.getSize());
    child.setName(this.code.getFirstSymbol());

    this.locationHint.applyTo(child);

    this.location = child;
  }

  collectSymbols(symbols) {
    const address = this.location.getMemoryAddress().getApplicationAddress();

    for (let symbol of this.code.getSymbols()) {
      symbols.add(symbol.name, symbol.offset + address);
    }
  }

  fillCalculations(symbols) {
    this.code.fillCalculations(symbols);
  }

  getRomOffset() {
    return this.location.getMemoryAddress().getRomOffset();
  }

  getBuffer() {
    return this.code.getCode();
  }
}
