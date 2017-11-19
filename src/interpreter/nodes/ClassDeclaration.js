import AbstractInterpreter from './AbstractInterpreter';
import ClassObject from '../../objects/Class';

export default class ClassDeclaration extends AbstractInterpreter {
  process() {

    // Finds the class
    var node = this.context.withoutParents().process(this.node.name);
    const klass = new ClassObject(node.getName());

    if (node.isUndefined()) {
      node.define(klass);
    } else if (node.getObjectType() !== 'Class') {
      this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Class.`);
      node.define(klass);
    }

    for (let extend of this.node.extendedClasses.getChildren()) {
      const parent = this.context.resolve(extend).getObject();

      if (parent.type() !== 'Class') {
        this.log('error', `The class ${node.getName()} tries to extend an object, which is no class`);
        continue;
      }

      klass.extendsClass(parent);
    }

    // Processes all children
    const context = this.context.enter(node.getObject());
    return context.processMany(this.node.getChildren());
  }
}
