import RootObject from '../objects/Root';
import Context from './Context';

export default class Interpreter {
  constructor(project) {
    this.project = project;
    this.root = new RootObject;

    this.functionQueue = [];
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
  }
}
