import InternalValueClass from './InternalValueClass';
import InternalValue from '../objects/InternalValue';
import PluginUtils from '../plugin/PluginUtils';

export default class NumberType extends InternalValueClass {
  constructor(parent) {
    super('Number', parent);
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'to_s', [], async (self, context) => {
      const value = new InternalValue(self.getMember('__value').value.toString());
      return await context.create('String', value);
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
