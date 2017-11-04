import ObjectClass from './Object';
import InternalValue from './InternalValue';
import PluginUtils from '../plugin/PluginUtils';
import Calculation from '../linker/calculate/Calculation';
import InterpreterError from '../interpreter/InterpreterError';

export default class FutureNumber extends ObjectClass {
  constructor(calculation) {
    super();

    this.calculation = calculation;
    this.initializeMembers();
  }

  type() {
    return 'FutureNumber';
  }

  getCalculation() {
    return this.calculation;
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('to_future_number', [], (self, context) => {
      return self;
    });

    klass.on('to_constant', [], (self, context) => {
      const type = context.create('String', new InternalValue('constant'));
      return context.create('TypedNumber', self, type);
    })

    this.initializeOperator(klass, '+');
    this.initializeOperator(klass, '-');
    this.initializeOperator(klass, '*');
    this.initializeOperator(klass, '/');
    this.initializeOperator(klass, '<<');
    this.initializeOperator(klass, '>>');
    this.initializeOperator(klass, '&');
    this.initializeOperator(klass, '|');
  }

  initializeOperator(klass, operator) {
    klass.on(operator, ['other'], (self, other, context) => {
      if (other.hasMember('to_n')) {
        other = other.getMember('to_n').callWithParameters(context.getContext());
      } else if (other.hasMember('to_future_number')) {
        other = other.getMember('to_future_number').callWithParameters(context.getContext());
      } else {
        throw new InterpreterError('Can not convert ' + other.type() + ' to Number');
      }

      return new FutureNumber(new Calculation(self, operator, other));
    })
  }
}
