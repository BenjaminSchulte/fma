import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import FunctionObject from '../../objects/Function';

export default class FunctionDeclaration extends AbstractInterpreter {
  async process() {

    var name = await this.context.getProcessor(this.node.name).asString();
    var node = await this.context.withoutParents().resolveChild(name);
    if (!node.isUndefined()) {
      this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Macro.`);
    }

    const func = new FunctionObject(name);
    func.setParentContext(this.context);
    func.setChildren(this.node.getChildren());

    node.define(func);

    return new ValueAccessor(func);

  }
}
