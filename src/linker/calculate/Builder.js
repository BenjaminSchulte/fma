import StaticNumber from './StaticNumber';
import SymbolLocation from './SymbolLocation';
import Calculation from './Calculation';

export default class CalculationBuilder {
  constructor(c=null) {
    this.calculation = c;
  }

  asBuilder(value) {
    switch (typeof value) {
      case 'number':
        return this.number(value);

      case 'string':
        return this.symbol(value);

      default:
        return value;
    }
  }

  number(v) {
    return new CalculationBuilder(new StaticNumber(v));
  }

  symbol(name) {
    return new CalculationBuilder(new SymbolLocation(name));
  }

  calc(op, other) {
    return new CalculationBuilder(new Calculation(this.calculation, op, this.asBuilder(other).get()));
  }

  add(other) {
    return this.calc('+', other);
  }

  sub(other) {
    return this.calc('-', other);
  }

  div(other) {
    return this.calc('/', other);
  }

  mul(other) {
    return this.calc('*', other);
  }

  get() {
    return this.calculation;
  }
}
