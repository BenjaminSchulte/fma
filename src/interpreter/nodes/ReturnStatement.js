import AbstractInterpreter from './AbstractInterpreter';
import ReturnError from '../ReturnError';
import {Instance as NilObject} from '../../objects/Nil';

export default class ReturnStatement extends AbstractInterpreter {
  process() {

    var result;
    if (this.node.parent) {
      result = (this.context.process(this.node.parent)).getObject();
    } else {
      result = NilObject;
    }

    throw new ReturnError(result);

  }
}
