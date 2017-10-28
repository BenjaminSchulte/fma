import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import BooleanObject from '../objects/Boolean';

export default class InternalValueClass extends Class {
  constructor(name, parent) {
    super(name);

    this.extendsClass(parent);
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

  operator(operator, callback) {
    PluginUtils.onInstance(this, operator, ['other'], async (self, other, context) => {
      if (!other.hasMember('to_s')) {
        throw new Error(`Can not compare String with ${other.type()}`)
      }

      const otherValue = await other.getMember('to_s').callWithParameters(context.getContext())
      if (otherValue.type() !== 'String') {
        throw new Error(`${other.type()}.to_s method must return a String`);
      }

      const left = self.getMember('__value').value;
      const right = otherValue.getMember('__value').value;

      return await callback(left, right, context);
    })
  }
}
