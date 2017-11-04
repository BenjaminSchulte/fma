import AbstractInterpreter from './AbstractInterpreter';
import Nil from '../../objects/Nil';
import ValueAccessor from '../ValueAccessor';

export default class NilLiteral extends AbstractInterpreter {
  process() {
    return new ValueAccessor(new Nil());
  }
}
