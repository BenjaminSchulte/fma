import Writer from './writer/Writer';
import LocationHint from './location/LocationHint';

export default class StaticCodeBlock {
  constructor(name) {
    this.name = name;
    this.code = new Writer();
    this.locationHint = new LocationHint();
    this.location = null;

    this.dependentFiles = {};
  }

  addDependency(file) {
    this.dependentFiles[file] = true;
  }

  getSize() {
    return this.code.size();
  }

  getLocationHint() {
    return this.locationHint;
  }

  isEmpty() {
    return this.getSize() == 0;
  }

  registerInMemory(memory) {
    if (this.isEmpty()) {
      return;
    }

    const child = memory.createChild();
    child.setItemSize(this.getSize());
    child.setName(this.code.getFirstSymbol());

    this.locationHint.applyTo(child);
    this.location = child;
  }

  collectSymbols(symbols) {
    if (this.isEmpty()) {
      this.collectSymbolsForEmpty(symbols);
      return;
    }

    if (!this.location.hasMemoryAddress()) {
      throw new Error('Could not find memory address for symbols: ' + this.code.getSymbols().map(symbol => symbol.name).join(', '));
      return;
    }

    const address = this.location.getMemoryAddress().getApplicationAddress();
    for (let symbol of this.code.getSymbols()) {
      symbols.add(symbol.name, symbol.offset + address);
    }
  }

  collectSymbolsForEmpty(symbols) {
    for (let symbol of this.code.getSymbols()) {
      symbols.add(symbol.name, 0);
    }
  }

  fillCalculations(symbols, linker) {
    this.code.fillCalculations(symbols, linker);
  }

  getRomOffset() {
    if (this.isEmpty()) {
      return 0;
    }

    return this.location.getMemoryAddress().getRomOffset();
  }

  getBuffer() {
    return this.code.getCode();
  }
}
