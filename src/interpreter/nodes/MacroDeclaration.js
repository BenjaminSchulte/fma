import AbstractInterpreter from './AbstractInterpreter';
import MacroObject from '../../objects/Macro';
import ArgumentList from '../ArgumentList';

export default class MacroDeclaration extends AbstractInterpreter {
  async process() {

    // Finds the macro
    const name = await this.context.getProcessor(this.node.name).asString();
    var node = await this.context.withoutParents().resolveChild(name);

    if (!node.isUndefined() && node.getObjectType() !== 'Macro') {
      this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Macro.`);
    }

    const macro = new MacroObject(node.getName());
    macro.setParentContext(this.context);
    macro.setArguments(await this.parseArguments(this.node.getArguments()));
    macro.setChildren(this.node.getChildren());

    if (node.getTargetType() === 'Class') {
      node.getTarget().setInstanceMember(node.getName(), macro);
    } else {
      node.define(macro);
    }
  }

  async parseArguments(args) {
    const list = new ArgumentList();

    for (let arg of args) {
      list.mergeArguments(await this.context.process(arg));
    }

    return list;
  }
}
