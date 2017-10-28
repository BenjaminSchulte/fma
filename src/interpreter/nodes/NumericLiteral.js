import AbstractInterpreter from './AbstractInterpreter';
import InternalValue from '../../objects/InternalValue';
import ValueAccessor from '../ValueAccessor';

export default class NumericLiteral extends AbstractInterpreter {
  async asNumber() {
    return this.node.number;
  }

  async process() {
    const str = (await this.context.getRoot().resolveChild("Number")).getObject();
    const value = new InternalValue(await this.asNumber());

    return new ValueAccessor(await str.getMember('new').callWithParameters(this.context, value));
  }
}
