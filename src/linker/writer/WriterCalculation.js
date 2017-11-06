export default class WriterCalculation {
  constructor(offset, calculation, size) {
    this.offset = offset;
    this.calculation = calculation;
    this.size = size;
  }

  calculate(symbols, linker) {
    return this.calculation.calculate(symbols, linker);
  }
}
