import AbstractInterpreter from './AbstractInterpreter';
import ChildValueAccessor from '../ChildValueAccessor';
import UndefinedObject from '../../objects/Undefined';
import InterpreterError from '../InterpreterError';

export default class ChildExpression extends AbstractInterpreter {
  async process() {

    const target = await this.context.resolve(this.node.parent);
    const child = await this.context.getProcessor(this.node.child).asString();

    const object = target.getObject();
    if (!object.hasMember(child)) {
      return new ChildValueAccessor(object, new UndefinedObject(), child);
      //throw new InterpreterError(`Could not find member "${child}" in ${target.getObjectName()}`)
    }

    const item = object.getMember(child);
    return new ChildValueAccessor(object, item, child);

  }
}
