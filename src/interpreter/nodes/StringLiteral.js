import AbstractInterpreter from './AbstractInterpreter';
import InternalValue from '../../objects/InternalValue';
import ValueAccessor from '../ValueAccessor';

export default class StringLiteral extends AbstractInterpreter {
  async asString() {
    return await this.context.getProcessor(this.node.identifier).asString();
  }

  async process() {
    const str = (await this.context.getRoot().resolveChild("String")).getObject();
    const value = new InternalValue(await this.asString());

    return new ValueAccessor(await str.getMember('new').callWithParameters(this.context, value));
  }
}
