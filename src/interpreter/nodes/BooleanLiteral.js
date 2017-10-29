import AbstractInterpreter from './AbstractInterpreter';
import BooleanObject from '../../objects/Boolean';
import ValueAccessor from '../ValueAccessor';

export default class BooleanLiteral extends AbstractInterpreter {
  async process() {
    return new ValueAccessor(new BooleanObject(this.node.value));
  }
}
