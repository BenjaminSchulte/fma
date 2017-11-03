import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import UndefinedObject from '../../objects/Undefined';

export default class IfStatement extends AbstractInterpreter {
  async process() {

    const condition = await this.context.resolve(this.node.condition);

    if (await condition.asBoolean(this.context, this.log.bind(this))) {
      return await this.context.processMany(this.node.getChildren());
    } else if (this.node.otherwise && this.node.otherwise.type() === 'ElseStatement') {
      return await this.context.processMany(this.node.otherwise.getChildren());
    } else if (this.node.otherwise && this.node.otherwise.type() === 'IfStatement') {
      return await this.context.process(this.node.otherwise);
    } else {
      return new ValueAccessor(new UndefinedObject());
    }

  }
}
