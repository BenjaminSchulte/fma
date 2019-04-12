const fma = require('./index');

const project = new fma.Project("65816");
project.addIncludeDirectory(".");

const parser = new fma.Parser(project);

console.log(project);
console.log(parser);
console.log(parser.parseFile("./tests/fma/array.fma"))
console.log(parser.getRootNode());