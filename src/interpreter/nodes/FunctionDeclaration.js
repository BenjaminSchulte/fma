import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import FunctionObject from '../../objects/Function';

export default class FunctionDeclaration extends AbstractInterpreter {
  process() {

    var name = this.context.getProcessor(this.node.name).asString();
    var node = this.context.withoutParents().resolveChild(name);
    if (!node.isUndefined()) {
      this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Macro.`);
    }

    const func = new FunctionObject(name);
    func.setParentContext(this.context);
    func.setChildren(this.node.getChildren());

    for (let decorator of this.node.getDecorators()) {
      this.context.process(decorator);
    }

    node.define(func);

    this.context.enter(func).preprocessMany(this.node.getChildren());

    func.getDocumentation().add(this.comments);

    return new ValueAccessor(func);

  }
}
