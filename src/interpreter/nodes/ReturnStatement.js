import AbstractInterpreter from './AbstractInterpreter';
import ReturnError from '../ReturnError';
import NilObject from '../../objects/Nil';

export default class ReturnStatement extends AbstractInterpreter {
  async process() {

    var result;
    if (this.node.parent) {
      result = (await this.context.process(this.node.parent)).getObject();
    } else {
      result = new NilObject();
    }

    throw new ReturnError(result);

  }
}
