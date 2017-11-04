import AbstractInterpreter from './AbstractInterpreter';
import ClassObject from '../../objects/Class';

export default class ClassDeclaration extends AbstractInterpreter {
  async process() {

    // Finds the class
    var node = await this.context.process(this.node.name);
    if (node.isUndefined()) {
      node.define(new ClassObject(node.getName()));
    } else if (node.getObjectType() !== 'Class') {
      this.log('warning', `The object ${node.getName()} has already been defined, but is redefined as Class.`);
      node.define(new ClassObject(node.getName()));
    }

    // Processes all children
    const context = this.context.enter(node.getObject());
    return await context.processMany(this.node.getChildren());
  }
}
