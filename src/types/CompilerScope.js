import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import Writer from '../linker/writer/Writer';
import InterpreterError from '../interpreter/InterpreterError';

export default class CompilerScope extends Class {
  constructor() {
    super('CompilerScope');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('on_enter_function', ['function'], () => {})
    klass.on('on_call_function', ['function'], () => {})
    klass.on('on_leave_function', ['function'], () => {})

    klass.on('initialize', [], (self, context) => {
      self.writer = new Writer();
    })

    klass.on('dw', ['*args', '**kwargs'], (self, args, kwargs, context) => {
      this.write(self, context, args.getItems(), kwargs.getItems(), 16);
    })

    klass.on('db', ['*args', '**kwargs'], (self, args, kwargs, context) => {
      this.write(self, context, args.getItems(), kwargs.getItems(), 8);
    })
  }

  write(self, context, items, options, bytesPerItem) {
    var numItems = 0;

    for (let item of items) {
      if (item.getClassName() === 'String') {
        const str = item.getMember('__value').getValue();
        for (let item of str.split('')) {
          self.writer.write(item.charCodeAt(0), bytesPerItem);
          numItems++;
        }
      } else {
        this.writeItem(self, context, item, bytesPerItem);
        numItems++;
      }
    }
  }

  writeItem(self, context, item, bytesPerItem) {
    if (item.getClassName() === 'FutureNumber') {
      self.writer.writeCalculation(item.getCalculation(), bytesPerItem);
      return;
    }

    if (item.hasMember('to_n')) {
      const number = item.getMember('to_n').callWithParameters(context.getContext());
      self.writer.write(number.getMember('__value').getValue(), bytesPerItem);
    } else if (item.hasMember('to_future_number')) {
      const number = item.getMember('to_future_number').callWithParameters(context.getContext());
      self.writer.write(number.getCalculation(), bytesPerItem);
    } else {
      throw new InterpreterError(`Unable to convert type ${item.getClassName()} to Number`);
    }

  }
}
