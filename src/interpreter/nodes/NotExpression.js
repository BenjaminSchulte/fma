import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import BooleanObject from '../../objects/Boolean';

export default class NotExpression extends AbstractInterpreter {
  process() {
//////    console.log('TODO: NotExpression should call member')

    const condition = this.context.resolve(this.node.parent);
    var result = condition.asBoolean(this.context, this.log.bind(this));

    return new ValueAccessor(new BooleanObject(!result));
  }

}
