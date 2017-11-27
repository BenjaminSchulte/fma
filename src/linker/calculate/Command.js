import BaseCalculation from './BaseCalculation';

export default class Command extends BaseCalculation {
  constructor(parts, returnString=false) {
    super();

    this.parts = parts;
    this.returnString = returnString;
  }

  calculate(symbols, linker) {
    const text = this.buildText(symbols, linker);

    if (this.returnString) {
      return text;
    } else {
      return linker.addCommand(text);
    }
  }

  buildText(symbols, linker) {
    return this.parts.map((part) => {
      if (typeof part === 'string') {
        return part;
      } else {
        return part.calculate(symbols, linker).toString(16);
      }
    }).join('')
  }

  toString() {
    return JSON.stringify(this.parts);
  }
}
