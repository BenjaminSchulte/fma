import AbstractFileSource from './AbstractFileSource';
const fs = require('fs');
const path = require('path');

export default class DirectorySource extends AbstractFileSource {
  constructor(dir) {
    super();

    this.dir = dir;
  }

  parseFile(parser, file) {
    file = path.join(this.dir, file);

    if (!fs.existsSync(file)) {
      return false;
    }

    return parser.parseFileAbsolute(file);
  }
}
