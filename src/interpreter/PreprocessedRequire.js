import Parser from '../grammar/Parser';
import async from 'async';
import path from 'path';

export default class PreprocessedRequire {
  constructor(project) {
    this.parsedFiles = {};
    this.parser = new Parser(project);
  }

  parseRelativeFile(file, searchPath) {
    const hash = `${searchPath}:${file}`;

    if (this.parsedFiles.hasOwnProperty(hash)) {
      return this.parsedFiles[hash];
    }

    const program = this.parser.parseRelativeFile(file, searchPath);
    this.parsedFiles[hash] = program;
    this.preprocess(program);

    return program;
  }

  parseString(node) {
    if (node.type() !== 'StringLiteral') {
      return null;
    }

    var string = "";
    for (let child of node.identifier.getChildren()) {
      if (child.type() !== 'IdentifierName') {
        return null;
      }

      string += child.name;
    }

    return string;
  }

  preprocessRequire(node) {
    const fileName = this.parseString(node.parent);
    const searchPath = path.dirname(node.getLocation().getFile());

    if (fileName === null) {
      return;
    }

    this.parseRelativeFile(fileName + ".fma", searchPath);
  }

  preprocess(node) {
    switch (node.type()) {
    case 'Program':
      this.preprocessMany(node.getChildren())
      break;

    case 'RequireStatement':
      this.preprocessRequire(node);
      break;

    default:
      break;
    }
  }

  preprocessMany(nodes) {
    async.forEachOf(nodes, (node, key, value) => {
      this.preprocess(node);
    })
  }

}
