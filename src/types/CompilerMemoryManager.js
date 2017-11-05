import Class from '../objects/Class';
import ClassInstance from '../objects/ClassInstance';
import FutureNumber from '../objects/FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import PluginUtils from '../plugin/PluginUtils';

import MemoryAllocation from '../memory/MemoryAllocation';
import RootMemoryAllocation from '../memory/RootMemoryAllocation';

export default class CompilerMemoryManager extends Class {
  constructor(interpreter) {
    super('CompilerMemoryManager');

    this.interpreter = interpreter;
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('initialize', ['?parent'], (self, parent, context) => {
      var memory;
      if (parent.isNil()) {
        memory = new RootMemoryAllocation();
      } else if (parent.getClassName() === 'CompilerMemoryManager') {
        memory = parent.memory.createChild();
      } else {
        throw new Error('Parent of memory allocation must be CompilerMemoryManager');
      }

      self.memory = memory;
    })

    klass.on('allocate', [], (self, context) => {
      const instance = context.create('CompilerMemoryManager', self);

      return instance;
    })

    klass.on('allocate_shadow', [], (self, context) => {
      const instance = context.create('CompilerMemoryManager', self);

      return instance;
    })

    klass.on('allow_range', ['range', 'address_and', 'address_or', 'align'], (self, range, addressAnd, addressOr, align, context) => {
      var rangeFrom = range.isNil() ? null : range.left;
      var rangeTo = range.isNil() ? null : range.right;

      addressAnd = addressAnd.isNil() ? null : context.asNumber(addressAnd);
      addressOr = addressOr.isNil() ? null : context.asNumber(addressOr);
      align = align.isNil() ? null : context.asNumber(align);

      self.memory.allowRange(rangeFrom, rangeTo, addressAnd, addressOr, align);
    })

    klass.on('to_future_number', [], (self, context) => {
      return new FutureNumber(new SymbolLocation(self.getSymbolName()));
    })
  }
}
