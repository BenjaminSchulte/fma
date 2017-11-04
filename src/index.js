import Parser from './grammar/Parser';
import Project from './project/Project';
import Interpreter from './interpreter/Interpreter';
import CorePlugin from './types/CorePlugin';

import {Assembler} from './asm/snes65816';

const asm = new Assembler();
const project = new Project(asm);
const parser = new Parser(project);
const interpreter = new Interpreter(project);

const parse = (name) => {
  project.log('info', `Parsing file ${name}`);

  const program = parser.parseFile(name);
  interpreter.process(program);

  project.log('info', `Done`);
}

const run = () => {
  interpreter.loadPlugin(new CorePlugin());

  parse('./src/core.pasm');
  parse('./src/asm/snes65816/snes65816.pasm');
  parse('./engine.pasm');
  parse('./test.pasm');
}

process.on('unhandledRejection', (reason) => {
    console.error(reason);
});

console.log(run());
