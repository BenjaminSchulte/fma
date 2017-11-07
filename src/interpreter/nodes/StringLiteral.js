import AbstractInterpreter from './AbstractInterpreter';
import InternalValue from '../../objects/InternalValue';
import ValueAccessor from '../ValueAccessor';

export default class StringLiteral extends AbstractInterpreter {
  asString() {
    return this.context.getProcessor(this.node.identifier).asString();
  }

  process() {
    const str = (this.context.getRoot().resolveChild("String")).getObject();
    const value = new InternalValue(this.asString());

    return new ValueAccessor(this.callWithParameters(str.getMember('new'), value));
  }
}
