import AbstractInterpreter from './AbstractInterpreter';
import {Instance as Nil} from '../../objects/Nil';
import ValueAccessor from '../ValueAccessor';

export default class NilLiteral extends AbstractInterpreter {
  process() {
    return new ValueAccessor(Nil);
  }
}
