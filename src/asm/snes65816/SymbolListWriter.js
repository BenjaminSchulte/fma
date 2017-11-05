export default class SymbolListWriter {
  constructor(symbols) {
    this.symbols = symbols;
  }

  write() {
    var result = [];

    var pad = "0000"

    result.push('[labels]');
    const symbols = this.symbols.all();
    for (let name in symbols) {
      const fullAddress = symbols[name];

      const bank = (fullAddress >> 16).toString(16);
      const address = (fullAddress & 0xFFFF).toString(16);

      result.push(`${pad.substr(0,2-bank.length)}${bank}:${pad.substr(0,4-address.length)}${address} ${name}`);
    }
    return result.join("\n")
  }
}
