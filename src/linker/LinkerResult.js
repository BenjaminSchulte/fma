export default class LinkerResult {
  constructor(binary, symbols, romCalculator) {
    this.binary = binary;
    this.symbols = symbols;
    this.romCalculator = romCalculator;
  }

  getBinary() {
    return this.binary;
  }

  getSymbols() {
    return this.symbols;
  }

  getRomCalculator() {
    return this.romCalculator;
  }
}
