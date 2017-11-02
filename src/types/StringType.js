import InternalValueClass from './InternalValueClass';
import PluginUtils from '../plugin/PluginUtils';
import InterpreterError from '../interpreter/InterpreterError';
import InternalValue from '../objects/InternalValue';

export default class StringType extends InternalValueClass {
  constructor() {
    super('String');
  }

  getConvertMethodName() {
    return 'to_s';
  }

  getTargetType() {
    return 'String'
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'to_s', [], async (self) => {
      return self;
    })

    PluginUtils.onInstance(this, 'upcase', [], async (self, context) => {
      return await context.create('String', new InternalValue(self.getMember('__value').getValue().toUpperCase()));
    })

    PluginUtils.onInstance(this, 'downcase', [], async (self, context) => {
      return await context.create('String', new InternalValue(self.getMember('__value').getValue().toLowerCase()));
    })

    this.operatorBoolean('==', (a, b) => { return a == b; });
  }
}
