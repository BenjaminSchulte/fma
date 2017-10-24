export default class Location {
  constuctor(line, column) {
    this.file = Location.currentFile;
    this.line = line;
    this.column = column;
  }

  static setCurrentFile(currentFile) {
    Location.currentFile = currentFile;
  }
}

Location.currentFile = null;
