import RootObject from '../objects/Root';
import Context from './Context';
import LinkerObject from '../linker/LinkerObject';

export default class Interpreter {
  constructor(project) {
    this.project = project;
    this.root = new RootObject;

    this.staticCodeBlocks = [];
    this.ramBlocks = [];
    this.romBlocks = [];

    this.functionQueue = [];
    this.callQueue = [];
  }

  afterwards(callback) {
    this.callQueue.push(callback);
  }

  registerStaticCodeBlock(block) {
    this.staticCodeBlocks.push(block);
  }

  compileFunction(func) {
    this.functionQueue.push(func);
  }

  getProject() {
    return this.project;
  }

  log(...args) {
    this.project.log(...args);
  }

  loadPlugin(plugin) {
    plugin.register(this.root, this);
  }

  process(program) {
    const context = new Context(this, this.root);

    context.process(program);

    while (this.functionQueue.length) {
      const func = this.functionQueue.shift();
      func.compile(context);
    }

    while (this.callQueue.length) {
      this.callQueue.shift()();
    }
  }

  addRomBlock(memory) {
    this.romBlocks.push(memory);
  }

  addRamBlock(memory) {
    this.ramBlocks.push(memory);
  }

  buildObject() {
    const object = new LinkerObject();

    for (let block of this.staticCodeBlocks) {
      object.addStaticCode(block);
    }

    for (let block of this.romBlocks) {
      object.addRomBlock(block);
    }

    for (let block of this.ramBlocks) {
      object.addRamBlock(block);
    }

    return object;
  }
}
