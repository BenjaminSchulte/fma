import ObjectClass from './Object';
import Context from '../interpreter/Context';
import ArgumentList from '../interpreter/ArgumentList'
import InterpreterError from '../interpreter/InterpreterError'

export default class ClassInstanceObject extends ObjectClass {
  constructor(klass) {
    super();

    this.klass = klass;
  }

  type() {
    return 'ClassInstance';
  }

  getName() {
    return this.klass.getName();
  }

  getClassName() {
    return this.klass.getName();
  }

  hasMember(name) {
    if (super.hasMember(name)) {
      return true;
    }

    return this.klass.hasInstanceMember(name);
  }

  async asString(context) {
    if (!this.hasMember('to_s')) {
      throw new InterpreterError('Could not convert object to string');
    }

    const str = await this.getMember('to_s').callWithParameters(context);
    return str.getMember('__value').getValue();
  }

  getMember(name) {
    if (super.hasMember(name)) {
      return super.getMember(name);
    }

    const member = this.klass.getInstanceMember(name, this);
    return member;
  }
}
