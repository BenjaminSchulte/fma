import BaseCalculation from './BaseCalculation';

export default class Calculation extends BaseCalculation {
  constructor(left, operator, right) {
    super();

    this.left = left;
    this.operator = operator;
    this.right = right;
  }

  calculate(symbols, linker) {
    const left = this.left.calculate(symbols, linker);
    const right = this.right.calculate(symbols, linker);

    switch (this.operator) {
    case '+': return left + right;
    case '-': return left - right;
    case '/': return left / right;
    case '*': return left * right;
    case '<<': return left << right;
    case '>>': return left >> right;
    case '&': return left & right;
    case '|': return left | right;
    default:
      throw new Error('Unsupported operator for Calculation: ' + operator);
    }
  }

  toString() {
    return `(${this.left.toString()}${this.operator}${this.right.toString()})`;
  }
}
