import DynamicBuffer from './DynamicBuffer';
import WriterCalculation from './WriterCalculation';
import WriterSymbol from './WriterSymbol';

export default class Writer {
  constructor() {
    this.code = new DynamicBuffer();

    this.symbols = [];
    this.calculations = [];
  }

  getCode() {
    return this.code;
  }

  fillCalculations(symbols, linker) {
    for (let calculation of this.calculations) {
      var value = calculation.calculate(symbols, linker);

      const buffer = this.code.buffer;
      for (let i=0; i<calculation.size; i++) {
        buffer[calculation.offset + i] = value & 0xFF;
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
      item >>= 8;
    }
  }

  writeBuffer(buffer) {
    this.code.write(buffer);
  }

  writeCalculation(calculation, bytesPerItem) {
    this.calculations.push(new WriterCalculation(this.size(), calculation, bytesPerItem));

    while (bytesPerItem--) {
      this.code.writeUInt8(0x00);
    }
  }
}
