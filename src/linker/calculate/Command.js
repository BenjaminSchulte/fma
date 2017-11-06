import BaseCalculation from './BaseCalculation';

export default class Command extends BaseCalculation {
  constructor(command) {
    super();

    this.command = command;
  }

  calculate(symbols, linker) {
    return linker.addCommand(this.command);
  }

  toString() {
    return JSON.stringify(this.command);
  }
}
