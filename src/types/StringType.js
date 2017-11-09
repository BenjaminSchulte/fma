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

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('to_s', [], (self) => {
      return self;
    })

    klass.on('to_n', [], (self, context) => {
      const number = parseInt(self.getMember('__value').getValue(), 10);
      if (isNaN(number)) {
        throw new InterpreterError('Can not convert String to Number: Invalid number');
      }
      return context.returnNumber(number);
    })

    klass.on('upcase', [], (self, context) => {
      return context.returnString(self.getMember('__value').getValue().toUpperCase());
    })

    klass.on('classify', [], (self, context) => {
      var value = self.getMember('__value').getValue();
      value = value[0].toUpperCase() + value.substr(1).replace(/_([a-z])/g, function(a, b) { return b.toUpperCase()});

      return context.returnString(value);
    })

    klass.on('downcase', [], (self, context) => {
      return context.returnString(self.getMember('__value').getValue().toLowerCase());
    })

    this.operatorBoolean(klass, '==', (a, b) => { return a == b; });
  }
}
