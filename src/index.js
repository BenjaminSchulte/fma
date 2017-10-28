import Parser from './grammar/Parser';
import Project from './project/Project';
import Interpreter from './interpreter/Interpreter';
import CorePlugin from './types/CorePlugin';

import {Assembler} from './asm/snes65816';

const asm = new Assembler();
const project = new Project(asm);
const parser = new Parser(project);
const interpreter = new Interpreter(project);

const parse = async (name) => {
  project.log('info', `Parsing file ${name}`);

  const program = await parser.parseFile(name);
  await interpreter.process(program);

  project.log('info', `Done`);
}

const run = async () => {
  await interpreter.loadPlugin(new CorePlugin());

  await parse('./src/core.pasm');
  await parse('./src/asm/snes65816/snes65816.pasm');
}

process.on('unhandledRejection', (reason) => {
    console.error(reason);
});

run().then((result) => {
  console.log(result);
}).catch((err) => {
  console.error(err);
})
