import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import NilObject from '../../objects/Nil';

export default class IfStatement extends AbstractInterpreter {
  process() {

    const condition = this.context.resolve(this.node.condition);

    if (condition.asBoolean(this.context, this.log.bind(this))) {
      return this.context.processMany(this.node.getChildren());
    } else if (this.node.otherwise && this.node.otherwise.type() === 'ElseStatement') {
      return this.context.processMany(this.node.otherwise.getChildren());
    } else if (this.node.otherwise && this.node.otherwise.type() === 'IfStatement') {
      return this.context.process(this.node.otherwise);
    } else {
      return new ValueAccessor(new NilObject());
    }

  }
}
