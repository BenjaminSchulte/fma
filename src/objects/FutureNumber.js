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

    klass.on('to_constant', [], async (self, context) => {
      const type = await context.create('String', new InternalValue('constant'));
      return await context.create('TypedNumber', self, type);
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
    klass.on(operator, ['other'], async (self, other, context) => {
      if (other.hasMember('to_n')) {
        other = await other.getMember('to_n').callWithParameters(context.getContext());
      } else if (other.hasMember('to_future_number')) {
        other = await other.getMember('to_future_number').callWithParameters(context.getContext());
      } else {
        throw new InterpreterError('Can not convert ' + other.type() + ' to Number');
      }

      return new FutureNumber(new Calculation(self, operator, other));
    })
  }
}
