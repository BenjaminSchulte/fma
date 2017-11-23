import AbstractInterpreter from './AbstractInterpreter';
import MacroObject from '../../objects/Macro';
import ArgumentList from '../ArgumentList';
import ValueAccessor from '../ValueAccessor';
import MacroPointer from '../../objects/MacroPointer';

export default class MacroDeclaration extends AbstractInterpreter {
  process() {

    // Finds the macro
    var name = null;
    var node = null;
    if (this.node.name) {
      name = this.context.getProcessor(this.node.name).asString();

      if (this.node.isRoot) {
        node = this.context.getRoot().resolveChild(name);
      } else {
        node = this.context.withoutParents().resolveChild(name);
      }
      if (!node.isUndefined() && node.getObjectType() !== 'Macro') {
        this.log('warn', `The object ${node.getName()} has already been defined, but is redefined as Macro.`);
      }
    } else {
      name = `<inlinemacro:${MacroDeclaration.nextInlineBlockId++}>`;
    }

    const macro = new MacroObject(name);
    macro.setParentContext(this.context);
    macro.setArguments(this.parseArguments(this.node.getArguments()));
    macro.setChildren(this.node.getChildren());
    macro.setIsDecorator(this.node.isDecorator);
    macro.setLocations(this.context.getParentLocations().concat(this.node.getLocation()));

    for (let decorator of this.node.getDecorators()) {
      this.context.enter(macro).process(decorator);
    }

    if (node) {
      if (node.getTargetType() === 'Class') {
        node.getTarget().setInstanceMember(node.getName(), macro);
      } else {
        node.define(macro);
      }
    }

    macro.getDocumentation().add(this.comments);

    return new ValueAccessor(new MacroPointer(macro));
  }

  parseArguments(args) {
    const list = new ArgumentList();

    for (let arg of args) {
      list.mergeArguments(this.context.process(arg));
    }

    return list;
  }
}

MacroDeclaration.nextInlineBlockId = 1;
