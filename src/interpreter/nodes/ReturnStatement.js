import AbstractInterpreter from './AbstractInterpreter';
import ReturnError from '../ReturnError';
import NilObject from '../../objects/Nil';

export default class ReturnStatement extends AbstractInterpreter {
  process() {

    var result;
    if (this.node.parent) {
      result = (this.context.process(this.node.parent)).getObject();
    } else {
      result = new NilObject();
    }

    throw new ReturnError(result);

  }
}
