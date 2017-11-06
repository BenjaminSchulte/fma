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

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('to_s', [], (self, context) => {
      const value = new InternalValue(self.getMember('__value').value.toString());
      return context.create('String', value);
    })

    klass.on('to_n', [], (self, context) => {
      return self;
    })

    klass.on('to_constant', [], (self, context) => {
      const type = context.create('String', new InternalValue('constant'));
      return context.create('TypedNumber', self, type);
    })

    klass.on('to_b', [], (self, context) => {
      const value = self.getMember('__value').value;
      return new BooleanObject(value != 0);
    })

    this.operatorBoolean(klass, '==', (a, b) => { return a == b; });
    this.operatorBoolean(klass, '<', (a, b) => { return a < b; });
    this.operatorBoolean(klass, '>', (a, b) => { return a > b; });

    this.operatorNumeric(klass, '+', (a, b) => { return a + b; });
    this.operatorNumeric(klass, '-', (a, b) => { return a - b; });
    this.operatorNumeric(klass, '*', (a, b) => { return a * b; });
    this.operatorNumeric(klass, '/', (a, b) => { return a / b; });
    this.operatorNumeric(klass, '&', (a, b) => { return a & b; });
    this.operatorNumeric(klass, '|', (a, b) => { return a | b; });
    this.operatorNumeric(klass, '<<', (a, b) => { return a << b; });
    this.operatorNumeric(klass, '>>', (a, b) => { return a >> b; });
  }

  operatorNumeric(klass, operator, callback) {
    this.operator(klass, operator, (a, b, context) => {
      const result = callback(a, b);

      return this.getMember('new').callWithParameters(context.getContext(), new InternalValue(result));
    })
  }
}
