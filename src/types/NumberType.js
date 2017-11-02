import InternalValueClass from './InternalValueClass';
import InternalValue from '../objects/InternalValue';
import PluginUtils from '../plugin/PluginUtils';
import BooleanObject from '../objects/Boolean';

export default class NumberType extends InternalValueClass {
  constructor() {
    super('Number');
  }

  getConvertMethodName() {
    return 'to_n';
  }

  getTargetType() {
    return 'Number'
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'to_s', [], async (self, context) => {
      const value = new InternalValue(self.getMember('__value').value.toString());
      return await context.create('String', value);
    })

    PluginUtils.onInstance(this, 'to_n', [], async (self, context) => {
      return self;
    })

    PluginUtils.onInstance(this, 'to_constant', [], async (self, context) => {
      const type = await context.create('String', new InternalValue('constant'));
      return await context.create('TypedNumber', self, type);
    })

    PluginUtils.onInstance(this, 'to_b', [], async (self, context) => {
      const value = self.getMember('__value').value;
      return new BooleanObject(value != 0);
    })

    this.operatorBoolean('==', (a, b) => { return a == b; });

    this.operatorNumeric('+', (a, b) => { return a + b; });
    this.operatorNumeric('-', (a, b) => { return a - b; });
    this.operatorNumeric('*', (a, b) => { return a * b; });
    this.operatorNumeric('/', (a, b) => { return a / b; });
    this.operatorNumeric('&', (a, b) => { return a & b; });
    this.operatorNumeric('|', (a, b) => { return a | b; });
    this.operatorNumeric('<<', (a, b) => { return a << b; });
    this.operatorNumeric('>>', (a, b) => { return a >> b; });
  }

  operatorNumeric(operator, callback) {
    this.operator(operator, async(a, b, context) => {
      const result = callback(a, b);

      return await this.getMember('new').callWithParameters(context.getContext(), new InternalValue(result));
    })
  }
}
