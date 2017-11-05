import Parser from './grammar/Parser';
import Project from './project/Project';
import Interpreter from './interpreter/Interpreter';
import Linker from './linker/Linker';
import CorePlugin from './types/CorePlugin';
import fs from 'fs';

import {Assembler, SymbolListWriter} from './asm/snes65816';

const asm = new Assembler();
const project = new Project(asm);
const parser = new Parser(project);
const interpreter = new Interpreter(project);
const linker = new Linker(project);

const parse = (name) => {
  project.log('info', `Parsing file ${name}`);

  const program = parser.parseFile(name);
  interpreter.process(program);
}

const run = () => {
  interpreter.loadPlugin(new CorePlugin());

  parse('./src/core.pasm');
  parse('./src/asm/snes65816/snes65816.pasm');
  parse('./engine.pasm');
  parse('./test.pasm');

  linker.addObject(interpreter.buildObject());

  var result = linker.link();
  result = asm.postProcess(result);

  return result;
}

process.on('unhandledRejection', (reason) => {
    console.error(reason);
});

const result = run();
fs.writeFileSync('test.sfc', result.getBinary())

const symbols = new SymbolListWriter(result.getSymbols());
fs.writeFileSync('test.sym', symbols.write());
