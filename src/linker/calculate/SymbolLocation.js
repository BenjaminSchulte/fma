import BaseCalculation from './BaseCalculation';

export default class SymbolLocation extends BaseCalculation {
  constructor(name) {
    super();

    this.name = name;
  }

  toString() {
    return this.name;
  }
}
