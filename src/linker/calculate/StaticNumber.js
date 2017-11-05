import BaseCalculation from './BaseCalculation';

export default class StaticNumber extends BaseCalculation {
  constructor(number) {
    super();

    this.number = number;
  }

  calculate(symbols) {
    return this.number;
  }

  toString() {
    return '$' + this.number.toString(16);
  }
}
