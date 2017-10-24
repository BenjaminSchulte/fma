import Location from './Location';

const grammar = require('./grammar');
const fs = require('fs');

export default class Parser {
  constructor(configuration) {
    this.configuration = configuration;
  }

  async parseString(content, fileName) {
    Location.setCurrentFile(fileName);

    const result = grammar.parse(content);

    return result;
  }

  async parseFile(file) {
    const content = fs.readFileSync(file, {encoding: 'utf-8'});
    if (content === undefined || content === null) {
      throw new Error(`Could not read from file: ${file}`);
    }

    return await this.parseString(content, file);
  }
}
