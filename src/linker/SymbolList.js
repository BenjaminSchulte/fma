export default class SymbolList {
  constructor() {
    this.symbols = {};
  }

  has(name) {
    return this.symbols.hasOwnProperty(name);
  }

  add(name, value) {
    if (this.has(name)) {
      throw new Error(`Symbol ${name} has been defined twice`);
    }

    this.symbols[name] = parseInt(value, 10);
  }

  get(name) {
    return this.symbols[name];
  }

  all() {
    return this.symbols;
  }
}
