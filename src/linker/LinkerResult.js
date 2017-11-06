export default class LinkerResult {
  constructor(binary, symbols, romCalculator, commands) {
    this.binary = binary;
    this.symbols = symbols;
    this.romCalculator = romCalculator;
    this.commands = commands;
  }

  clone() {
    return new LinkerResult(
      this.binary,
      this.symbols,
      this.romCalculator,
      this.commands
    )
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

  getCommands() {
    return this.commands;
  }
}
