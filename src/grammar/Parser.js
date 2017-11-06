import Location from './Location';

const grammar = require('./grammar');
const fs = require('fs');
const path = require('path');

export default class Parser {
  constructor(project) {
    this.project = project;
  }

  parseString(content, fileName="<unknown>") {
    var result;

    Location.setCurrentFile(fileName);

    try {
      result = grammar.parse(content);
    } catch(err) {
      var prefix = '';
      if (err.location) {
        prefix = (new Location(err.location.start.line, err.location.start.column)).toString() + ': ';
      }

      this.project.log('error', prefix + err.message);

      throw err;
    }

    return result;
  }

  parseFileAbsolute(file) {
    try {
      if (!fs.statSync(file).isFile()) {
        return null;
      }
    } catch(err) {
      return null;
    }

    return this.parseString(fs.readFileSync(file, {encoding: 'utf-8'}), file);
  }

  parseFile(file) {
    var result;
    if (result = this.parseFileAbsolute(file)) {
      return result;
    }

    for (let dir of this.project.getIncludeDirs()) {
      if (result = this.parseFileAbsolute(path.join(dir, file))) {
        return result;
      }
    }

    throw new Error(`Could not find include file: ${file}`);
  }

  parseRelativeFile(file, dir) {
    var result;
    if (result = this.parseFileAbsolute(path.join(dir, file))) {
      return result;
    }

    return this.parseFile(file);
  }
}
