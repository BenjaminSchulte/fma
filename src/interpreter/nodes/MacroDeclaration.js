import AbstractInterpreter from './AbstractInterpreter';
import MacroObject from '../../objects/Macro';
import ArgumentList from '../ArgumentList';
import ValueAccessor from '../ValueAccessor';
import MacroPointer from '../../objects/MacroPointer';

export default class MacroDeclaration extends AbstractInterpreter {
  async process() {

    // Finds the macro
    var name = null;
    var node = null;
    if (this.node.name) {
      name = await this.context.getProcessor(this.node.name).asString();

      node = await this.context.withoutParents().resolveChild(name);
      if (!node.isUndefined() && node.getObjectType() !== 'Macro') {
        this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Macro.`);
      }
    }

    const macro = new MacroObject(name);
    macro.setParentContext(this.context);
    macro.setArguments(await this.parseArguments(this.node.getArguments()));
    macro.setChildren(this.node.getChildren());

    if (node) {
      if (node.getTargetType() === 'Class') {
        node.getTarget().setInstanceMember(node.getName(), macro);
      } else {
        node.define(macro);
      }
    }

    return new ValueAccessor(new MacroPointer(macro));
  }

  async parseArguments(args) {
    const list = new ArgumentList();

    for (let arg of args) {
      list.mergeArguments(await this.context.process(arg));
    }

    return list;
  }
}
