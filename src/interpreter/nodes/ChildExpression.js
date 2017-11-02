import AbstractInterpreter from './AbstractInterpreter';
import ChildValueAccessor from '../ChildValueAccessor';
import UndefinedObject from '../../objects/Undefined';
import InterpreterError from '../InterpreterError';

export default class ChildExpression extends AbstractInterpreter {
  async process() {

    const target = await this.context.resolve(this.node.parent);
    var child;

    if (this.node.isResolved) {
      child = await (await this.context.resolve(this.node.child)).asString(this.context, this.log.bind(this));
    } else {
      child = await this.context.getProcessor(this.node.child).asString();
    }

    if (target.isUndefined()) {
      throw new InterpreterError(`Could not find member "${child}" in Undefined`)
    }

    const object = target.getObject();
    if (!object.hasMember(child)) {
      return new ChildValueAccessor(object, new UndefinedObject(), child);
    }

    const item = object.getMember(child);
    return new ChildValueAccessor(object, item, child);

  }
}
