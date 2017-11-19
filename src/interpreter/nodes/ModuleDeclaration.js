import AbstractInterpreter from './AbstractInterpreter';
import Module from '../../objects/Module';

export default class ModuleDeclaration extends AbstractInterpreter {
  process() {

    // Finds the module
    var context = this.context;
    const parts = this.node.name.getChildren();
    if (this.node.isRoot) {
      context = this.context.getRoot();
    }

    var object = null;
    for (let part of parts) {
      const name = this.context.getProcessor(part).asString();
      object = context.withoutParents().resolveChild(name);

      if (object.isUndefined()) {
        object.define(new Module(name));
      } else if (object.getObjectType() !== 'Module') {
        this.log('warn', `The object ${object.getName()} has already been defined, but is redefined as Module.`);
        object.define(new Module(name));
      }

      context = context.enter(object.getObject());
    }

    context.getObject().getDocumentation().add(this.comments);

    return context.processMany(this.node.getChildren());

  }
}
