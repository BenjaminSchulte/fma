import DynamicBuffer from './DynamicBuffer';
import WriterCalculation from './WriterCalculation';
import WriterSymbol from './WriterSymbol';

export default class Writer {
  constructor() {
    this.code = new DynamicBuffer();

    this.symbols = [];
    this.calculations = [];
  }

  fillCalculations(symbols) {
    for (let calculation of this.calculations) {
      var value = calculation.calculate(symbols);

      console.log(calculation.calculation.toString(), '=', value.toString(16));

      for (let i=0; i<calculation.size; i++) {
        this.code[calculation.offset + i] = value & 0xFF;
        value >>= 8;
      }
    }

    this.calculations = [];
  }

  getSymbols() {
    return this.symbols;
  }

  getFirstSymbol() {
    for (let symbol of this.symbols) {
      if (symbol.offset === 0) {
        return symbol.name;
      }
    }

    return null;
  }

  size() {
    return this.code.getLength();
  }

  writeSymbol(name) {
    this.symbols.push(new WriterSymbol(this.size(), name));
  }

  write(item, bytesPerItem) {
    while (bytesPerItem--) {
      this.code.writeUInt8(item & 0xFF);
      item <<= 8;
    }
  }

  writeCalculation(calculation, bytesPerItem) {
    this.calculations.push(new WriterCalculation(this.size(), calculation, bytesPerItem));

    while (bytesPerItem--) {
      this.code.writeUInt8(0x00);
    }
  }
}
