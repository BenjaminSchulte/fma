import RootObject from '../objects/Root';
import Context from './Context';

export default class Interpreter {
  constructor(project) {
    this.project = project;
    this.root = new RootObject;

    this.functions = [];
  }

  compileFunction(func) {
    this.functions.push(func);
  }

  getProject() {
    return this.project;
  }

  log(...args) {
    this.project.log(...args);
  }

  async loadPlugin(plugin) {
    await plugin.register(this.root, this);
  }

  async process(program) {
    const context = new Context(this, this.root);

    await context.process(program);

    while (this.functions.length) {
      const func = this.functions.shift();
      await func.compile(context);
    }
  }
}
