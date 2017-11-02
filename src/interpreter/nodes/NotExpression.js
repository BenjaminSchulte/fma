import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import BooleanObject from '../../objects/Boolean';

export default class NotExpression extends AbstractInterpreter {
  async process() {

    const condition = await this.context.resolve(this.node.parent);
    var result = await condition.asBoolean(this.context, this.log.bind(this));

    return new ValueAccessor(new BooleanObject(!result));
  }

}
