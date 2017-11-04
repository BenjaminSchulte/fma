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

  parseFile(file) {
    const content = fs.readFileSync(file, {encoding: 'utf-8'});
    if (content === undefined || content === null) {
      throw new Error(`Could not read from file: ${file}`);
    }

    return this.parseString(content, file);
  }

  parseRelativeFile(file, dir) {
    return this.parseFile(path.join(dir, file));
  }
}
