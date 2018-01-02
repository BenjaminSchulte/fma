import AbstractInterpreter from './AbstractInterpreter';
import ChildValueAccessor from '../ChildValueAccessor';
import {Instance as UndefinedObject} from '../../objects/Undefined';
import InterpreterError from '../InterpreterError';

export default class ChildExpression extends AbstractInterpreter {
  process() {

    const target = this.context.resolve(this.node.parent);
    var child;

    if (this.node.isResolved) {
      child = (this.context.resolve(this.node.child)).asString(this.context, this.log.bind(this));
    } else {
      child = this.context.getProcessor(this.node.child).asString();
    }

    if (target.isUndefined()) {
      throw new InterpreterError(`Could not find member "${child}" in Undefined`)
    }

    const object = target.getObject();
    if (!object.hasMember(child)) {
      return new ChildValueAccessor(object, UndefinedObject, child);
    }

    const item = object.getMember(child);
    return new ChildValueAccessor(object, item, child);

  }
}
