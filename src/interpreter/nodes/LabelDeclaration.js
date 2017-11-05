import AbstractInterpreter from './AbstractInterpreter';
import Label from '../../objects/Label';

export default class LabelDeclaration extends AbstractInterpreter {
  preprocess() {
    const func = this.context.getObject();
    const name = this.context.getProcessor(this.node.name).asString();

    const label = new Label(name);
    label.setParent(func);
    func.setMember(name, label);
  }

  process() {
    const name = this.context.getProcessor(this.node.name).asString();
    const scope = this.context.resolveChild('Compiler').getObject().getMember('current_scope');
    const label = this.context.resolveChild(name).getObject()

    if (label.type() !== 'Label') {
      throw new Error('Could not find label');
    }

    const symbolName = label.getSymbolName();
    scope.block.code.writeSymbol(symbolName);
  }
}
