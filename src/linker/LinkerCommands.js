export default class LinkerCommands {
  constructor() {
    this.commands = [];
  }

  add(command) {
    const index = this.commands.indexOf(command);
    if (index >= 0) {
      return index;
    }

    this.commands.push(command);
    return this.commands.length - 1;
  }

  all() {
    return this.commands;
  }
}
