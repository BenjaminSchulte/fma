export default class BaseCalculation {
  toString() {
    return 'BaseCalculation';
  }

  calculate(symbol) {
    throw new Error('BaseCalculation.calculate not implemented');
  }
}
