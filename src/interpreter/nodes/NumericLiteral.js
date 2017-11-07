import AbstractInterpreter from './AbstractInterpreter';
import InternalValue from '../../objects/InternalValue';
import ValueAccessor from '../ValueAccessor';

export default class NumericLiteral extends AbstractInterpreter {
  asNumber() {
    return this.node.number;
  }

  process() {
    const str = (this.context.getRoot().resolveChild("Number")).getObject();
    const value = new InternalValue(this.asNumber());

    return new ValueAccessor(this.callWithParameters(str.getMember('new'), value));
  }
}
