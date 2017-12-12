import ObjectClass from './Object';
import Context from '../interpreter/Context';
import ArgumentList from '../interpreter/ArgumentList'
import InterpreterError from '../interpreter/InterpreterError'
import FutureNumber from './FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';

export default class ClassInstanceObject extends ObjectClass {
  constructor(klass) {
    super();

    this.klass = klass;
  }

  type() {
    return 'ClassInstance';
  }

  setMemoryScope(scope) {
    this.memoryScope = scope;

    this.klass.instance.on('to_future_number', [], (self, context) => {
      if (!self.memoryScope) {
        throw new InterpreterError('Can not convert an instance which is no memory declaration to future number');
      }

      return new FutureNumber(new SymbolLocation(self.memoryScope.getSymbolName()));
    });
  }

  getName() {
    if (this.nameHint !== null) {
      return this.nameHint;
    }

    return this.klass.getName();
  }

  getClassName() {
    return this.klass.getName();
  }

  hasMember(name) {
    if (super.hasMember(name)) {
      return true;
    }

    return this.klass.hasInstanceMember(name, this);
  }

  asString(context) {
    if (!this.hasMember('to_s')) {
      throw new InterpreterError('Could not convert object to string');
    }

    const str = this.getMember('to_s').callWithParameters(context);
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
