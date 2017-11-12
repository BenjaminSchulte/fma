import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';
import StaticCodeBlock from '../linker/StaticCodeBlock';
import FutureNumber from '../objects/FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import InterpreterError from '../interpreter/InterpreterError';
import fs from 'fs';

export default class CompilerScope extends Class {
  constructor() {
    super('CompilerScope');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('on_enter_function', ['function'], () => {})
    klass.on('on_call_function', ['function'], () => {})
    klass.on('on_leave_function', ['function'], () => {})

    klass.on('initialize', ['function'], (self, func, context) => {
      self.block = new StaticCodeBlock();
      self.block.code.writeSymbol(func.getSymbolName());
      context.getInterpreter().registerStaticCodeBlock(self.block);
    })

    klass.on('PC', [], (self, context) => {
      const symbolName = `.pc${CompilerScope.nextSymbolId++}`;
      self.block.code.writeSymbol(symbolName);
      return new FutureNumber(new SymbolLocation(symbolName));
    })

    klass.on('dw', ['*args', '**kwargs'], (self, args, kwargs, context) => {
      this.write(self, context, args.getItems(), kwargs.getItems(), 2);
    })

    klass.on('db', ['*args', '**kwargs'], (self, args, kwargs, context) => {
      this.write(self, context, args.getItems(), kwargs.getItems(), 1);
    })

    klass.on('file', ['file'], (self, file, context) => {
      self.block.code.writeBuffer(fs.readFileSync(context.asString(file)));
    })

    klass.on('locate_at', ['range', 'address_and', 'address_or', 'align'], (self, range, addressAnd, addressOr, align, context) => {
      const location = self.block.getLocationHint();
      var rangeFrom = range.isNil() ? null : range.left;
      var rangeTo = range.isNil() ? null : range.right;

      addressAnd = addressAnd.isNil() ? null : context.asNumber(addressAnd);
      addressOr = addressOr.isNil() ? null : context.asNumber(addressOr);
      align = align.isNil() ? null : context.asNumber(align);

      location.allowRange(rangeFrom, rangeTo, addressAnd, addressOr, align)
    })
  }

  write(self, context, items, options, bytesPerItem) {
    var numItems = 0;

    for (let item of items) {
      if (item.getClassName() === 'String') {
        const str = item.getMember('__value').getValue();
        for (let item of str.split('')) {
          self.block.code.write(item.charCodeAt(0), bytesPerItem);
          numItems++;
        }
      } else {
        this.writeItem(self, context, item, bytesPerItem);
        numItems++;
      }
    }

    if (options.length) {
      const maxItems = context.asNumber(options.length);
      const fill = options.fill ? context.asNumber(options.fill) : 0;
      while (numItems++ < maxItems) {
        self.block.code.write(fill, 1)
      }
    }
  }

  writeItem(self, context, item, bytesPerItem) {
    if (item.getClassName() === 'FutureNumber') {
      self.block.code.writeCalculation(item.getCalculation(), bytesPerItem);
      return;
    }

    if (item.hasMember('to_n')) {
      const number = item.getMember('to_n').callWithParameters(context.getContext());
      self.block.code.write(number.getMember('__value').getValue(), bytesPerItem);
    } else if (item.hasMember('to_future_number')) {
      const number = item.getMember('to_future_number').callWithParameters(context.getContext());
      self.block.code.writeCalculation(number.getCalculation(), bytesPerItem);
    } else {
      throw new InterpreterError(`Unable to convert type ${item.getClassName()} to Number`);
    }

  }
}

CompilerScope.nextSymbolId = 1;
