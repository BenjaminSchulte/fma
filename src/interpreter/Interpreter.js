import RootObject from '../objects/Root';
import Context from './Context';

export default class Interpreter {
  constructor(project) {
    this.project = project;
    this.root = new RootObject;
  }

  getProject() {
    return this.project;
  }

  log(...args) {
    this.project.log(...args);
  }

  async loadPlugin(plugin) {
    await plugin.register(this.root);
  }

  async process(program) {
    const context = new Context(this, this.root);

    await context.process(program);
  }
}
