import RootObject from '../objects/Root';
import Context from './Context';
import LinkerObject from '../linker/LinkerObject';
import async from 'async';
import whilst from 'async/whilst';

import PreprocessedRequire from './PreprocessedRequire';
import os from 'os';

export default class Interpreter {
  constructor(project) {
    this.project = project;
    this.root = new RootObject;
    this.preprocessedRequire = new PreprocessedRequire(project);

    this.configurations = [];
    this.staticCodeBlocks = [];
    this.ramBlocks = [];
    this.romBlocks = [];

    this.functionQueue = [];
    this.callQueue = [];

    this.context = new Context(this, this.root);

    for (let plugin of project.getPlugins()) {
      plugin.preProcess(project, this);
    }
  }

  getRoot() {
    return this.root;
  }

  setCurrentBlock(block) {
    this.currentBlock = block;
  }

  getCurrentBlock() {
    return this.currentBlock;
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
    this.preprocessedRequire.preprocess(program);

    this.context.process(program);
  }

  addConfiguration(configuration) {
    this.configurations.push(configuration);
  }

  addRomBlock(memory) {
    this.romBlocks.push(memory);
  }

  addRamBlock(memory) {
    this.ramBlocks.push(memory);
  }

  finalize(callback) {
    whilst(
      () => {
        this.functionQueue = this.functionQueue.filter(func => !func.hasBeenCompile);
        return this.functionQueue.length;
      },
      (callback) => {
        var functions = this.functionQueue.slice();
        this.functionQueue = [];

        async.each(functions, (func, callback) => {
          func.compile(this.context);
          callback();
        }, callback);
      },
      (err) => {
        if (err) { return callback(err); }

        while (this.callQueue.length) {
          this.callQueue.shift()();
        }

        callback();
      }
    )
  }

  buildObject(callback) {
    this.finalize(() => {
      const object = new LinkerObject();

      for (let config of this.configurations) {
        object.addConfiguration(config);
      }

      for (let block of this.staticCodeBlocks) {
        object.addStaticCode(block);
      }

      for (let block of this.romBlocks) {
        object.addRomBlock(block);
      }

      for (let block of this.ramBlocks) {
        object.addRamBlock(block);
      }

      callback(null, object);
    });
  }
}
