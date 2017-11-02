import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import BooleanObject from '../objects/Boolean';

export default class InternalValueClass extends Class {
  constructor(name) {
    super(name);
  }

  initializeMembers() {
    super.initializeMembers();

    this.onInstance('initialize', ['value'], async (context) => {
      const self = (await context.resolveChild('self')).getObject();

      self.setMember('__value', (await context.resolveChild('value')).getObject());
    })
  }

  operatorBoolean(operator, callback) {
    this.operator(operator, async(a, b) => { return new BooleanObject(callback(a, b)); })
  }

  getConvertMethodName() {
    return '';
  }

  getTargetType() {
    return ''
  }

  operator(operator, callback) {
    PluginUtils.onInstance(this, operator, ['other'], async (self, other, context) => {
      if (!other.hasMember(this.getConvertMethodName())) {
        throw new Error(`Can not convert ${other.type()} to ${this.getTargetType()}`)
      }

      const otherValue = await other.getMember(this.getConvertMethodName()).callWithParameters(context.getContext())
      if (otherValue.type() !== this.getTargetType()) {
        throw new Error(`${other.type()}.${this.getConvertMethodName()} method must return a ${this.getTargetType()}`);
      }

      const left = self.getMember('__value').value;
      const right = otherValue.getMember('__value').value;

      return await callback(left, right, context);
    })
  }
}
