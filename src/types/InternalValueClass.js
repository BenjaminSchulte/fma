import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import BooleanObject from '../objects/Boolean';

export default class InternalValueClass extends Class {
  constructor(name) {
    super(name);
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('initialize', ['value'], async (self, value, context) => {
      self.setMember('__value', value);
    })
  }

  operatorBoolean(klass, operator, callback) {
    this.operator(klass, operator, async(a, b) => { return new BooleanObject(callback(a, b)); })
  }

  getConvertMethodName() {
    return '';
  }

  getTargetType() {
    return ''
  }

  operator(klass, operator, callback) {
    klass.on(operator, ['other'], async (self, other, context) => {
      if (!other.hasMember(this.getConvertMethodName())) {
        throw new Error(`Can not convert ${other.type()} to ${this.getTargetType()}`)
      }

      const otherValue = await other.getMember(this.getConvertMethodName()).callWithParameters(context.getContext())
      if (otherValue.type() !== 'ClassInstance' || otherValue.getClassName() !== this.getTargetType()) {
        throw new Error(`${other.type()}.${this.getConvertMethodName()} method must return a ${this.getTargetType()}`);
      }

      const left = self.getMember('__value').value;
      const right = otherValue.getMember('__value').value;

      return await callback(left, right, context);
    })
  }
}
