import AbstractInterpreter from './AbstractInterpreter';
import Module from '../../objects/Module';

export default class ModuleDeclaration extends AbstractInterpreter {
  process() {

    // Finds the module
    var node = this.context.withoutParents().process(this.node.name);
    if (node.isUndefined()) {
      node.define(new Module(node.getName()));
    } else if (node.getObjectType() !== 'Module') {
      this.log('warning', `The object ${node.getName()} has already been defined, but is redefined as Module.`);
      node.define(new Module(node.getName()));
    }

    // Processes all children
    const context = this.context.enter(node.getObject());
    return context.processMany(this.node.getChildren());

  }
}
