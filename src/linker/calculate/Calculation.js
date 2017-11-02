import BaseCalculation from './BaseCalculation';

export default class Calculation extends BaseCalculation {
  constructor(left, operator, right) {
    super();

    this.left = left;
    this.operator = operator;
    this.right = right;
  }

  toString() {
    return `(${this.left}${this.operator}${this.right})`;
  }
}
