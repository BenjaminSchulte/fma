import AbstractObject from './Object';
import BooleanObject from './Boolean';
import InterpreterError from '../interpreter/InterpreterError';

export default class UndefinedObject extends AbstractObject {
  constructor() {
    super();

    this.initializeMembers();
    this.canNotSetMember = true;
  }

  type() {
    return 'Undefined';
  }

  setMember(name, value) {
    if (this.canNotSetMember) {
      throw new InterpreterError('Can not set member of type Undefined');
    }

    super.setMember(name, value);
  }

  initializeMembers() {
    this.on('==', ['other'], async (context) => {
      const result = (await context.resolveChild('other')).isUndefined();
      return new BooleanObject(result);
    })
  }

  isNil() {
    return true;
  }

  isUndefined() {
    return true;
  }
}
