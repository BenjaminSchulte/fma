import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import Writer from '../linker/writer/Writer';
import InterpreterError from '../interpreter/InterpreterError';

export default class CompilerScope extends Class {
  constructor() {
    super('CompilerScope');
  }

  initializeMembers() {
    super.initializeMembers();

    this.onInstance('on_enter_function', ['function'], (context) => {})
    this.onInstance('on_leave_function', ['function'], (context) => {})

    PluginUtils.onInstance(this, 'initialize', [], async (self, context) => {
      self.writer = new Writer();
    })
    PluginUtils.onInstance(this, 'dw', ['*args', '**kwargs'], async (self, args, kwargs, context) => {
      await this.write(self, context, args.getItems(), kwargs.getItems(), 16);
    })
    PluginUtils.onInstance(this, 'db', ['*args', '**kwargs'], async (self, args, kwargs, context) => {
      await this.write(self, context, args.getItems(), kwargs.getItems(), 8);
    })
  }

  async write(self, context, items, options, bytesPerItem) {
    var numItems = 0;

    for (let item of items) {
      if (item.type() === 'String') {
        const str = item.getMember('__value').getValue();
        for (let item of str.split('')) {
          self.writer.write(item.charCodeAt(0), bytesPerItem);
          numItems++;
        }
      } else {
        await this.writeItem(self, context, item, bytesPerItem);
        numItems++;
      }
    }
  }

  async writeItem(self, context, item, bytesPerItem) {
    if (item.type() === 'FutureNumber') {
      self.writer.writeCalculation(item.getCalculation(), bytesPerItem);
      return;
    }

    if (!item.hasMember('to_n')) {
      throw new InterpreterError(`Unable to convert type ${item.type()} to Number`);
    }

    const number = await item.getMember('to_n').callWithParameters(context.getContext());
    self.writer.write(number.getMember('__value').getValue(), bytesPerItem);
  }
}
