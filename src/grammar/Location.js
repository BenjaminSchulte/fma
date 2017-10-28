export default class Location {
  constructor(line, column) {
    this.file = Location.currentFile;
    this.line = line;
    this.column = column;
  }

  getLine() {
    return this.line;
  }

  static setCurrentFile(currentFile) {
    Location.currentFile = currentFile;
  }

  toString() {
    return `${this.file}@${this.line}:${this.column}`;
  }
}

Location.currentFile = null;
