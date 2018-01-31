import InternalValueClass from './InternalValueClass';
import InternalValue from '../objects/InternalValue';
import PluginUtils from '../plugin/PluginUtils';
import BooleanObject from '../objects/Boolean';
import FutureNumber from '../objects/FutureNumber';
import StaticNumber from '../linker/calculate/StaticNumber';

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

  onOperator(self, operator, callback, other, context) {
    if (other.getClassName() !== 'Number') {
      if (!other.hasMember('to_n') && other.hasMember('to_future_number')) {
        other = other.getMember('to_future_number').callWithParameters(context.getContext());
      }
    }
    if (other.type() === 'FutureNumber') {
      self = new FutureNumber(new StaticNumber(context.asNumber(self)));
      return self.getMember(operator).callWithParameters(context.getContext(), other);
    }

    return super.onOperator(self, operator, callback, other, context);
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('to_s', ['?base'], (self, base, context) => {
      var baseValue = 10;
      if (!base.isNil()) {
        baseValue = context.asNumber(base);
      }

      const value = new InternalValue(self.getMember('__value').value.toString(baseValue));
      return context.create('String', value);
    })

    klass.on('to_n', [], (self, context) => {
      return self;
    })

    klass.on('times', ['&block'], (self, block, context) => {
      const macro = block.getMacro();
      const times = self.getMember('__value').value;

      for (var i=0; i<times; i++) {
        macro.callWithParameters(context.getContext(), context.create('Number', new InternalValue(i)));
      }
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
    this.operatorBoolean(klass, '!=', (a, b) => { return a != b; });
    this.operatorBoolean(klass, '<', (a, b) => { return a < b; });
    this.operatorBoolean(klass, '>', (a, b) => { return a > b; });

    this.operatorNumeric(klass, '^', (a, b) => { return a ^ b; });
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
