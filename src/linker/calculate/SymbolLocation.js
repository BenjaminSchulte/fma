import BaseCalculation from './BaseCalculation';

export default class SymbolLocation extends BaseCalculation {
  constructor(name) {
    super();

    this.name = name;
  }

  calculate(symbols, linker) {
    if (!symbols.has(this.name)) {
      throw new Error(`Could not resolve symbol: ${this.name}`);
    }

    return symbols.get(this.name);
  }

  toString() {
    return this.name;
  }
}
