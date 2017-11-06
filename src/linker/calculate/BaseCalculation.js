export default class BaseCalculation {
  toString() {
    return 'BaseCalculation';
  }

  calculate(symbols, linker) {
    throw new Error('BaseCalculation.calculate not implemented');
  }
}
