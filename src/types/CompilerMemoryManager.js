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
      this.interpreter.afterwards(() => {
        self.memory.setSymbolName(self.getSymbolName());
      });
    })

    klass.on('set_item_size', ['size'], (self, size, context) => {
      if (size.isNil()) {
        return;
      }

      self.memory.setItemSize(context.asNumber(size));
    });

    klass.on('set_item_type', ['type'], (self, type, context) => {
      if (type.isNil()) {
        return;
      }

      switch (context.asString(type)) {
        case 'uint8': self.memory.setItemSize(1); break;
        case 'uint16': self.memory.setItemSize(2); break;
        case 'uint24': self.memory.setItemSize(3); break;
        case 'uint32': self.memory.setItemSize(4); break;
        default:
          throw new Error(`Unknown item type: ` + type);
      }
    });

    klass.on('set_num_items', ['size'], (self, size, context) => {
      if (size.isNil()) {
        return;
      }

      self.memory.setNumItems(context.asNumber(size));
    });

    klass.on('allocate', [], (self, context) => {
      const instance = context.create('CompilerMemoryManager', self);

      return instance;
    })

    klass.on('allocate_shadow', [], (self, context) => {
      const instance = context.create('CompilerMemoryManager', self);

      return instance;
    })

    klass.on('allow_range', ['range', 'address_and', 'address_or', 'align', 'located_at'], (self, range, addressAnd, addressOr, align, locatedAt, context) => {
      var rangeFrom = range.isNil() ? null : range.left;
      var rangeTo = range.isNil() ? null : range.right;

      addressAnd = addressAnd.isNil() ? null : context.asNumber(addressAnd);
      addressOr = addressOr.isNil() ? null : context.asNumber(addressOr);
      locatedAt = locatedAt.isNil() ? null : context.asNumber(locatedAt);
      align = align.isNil() ? null : context.asNumber(align);

      self.memory.allowRange(rangeFrom, rangeTo, addressAnd, addressOr, align, locatedAt);
    })

    klass.on('shadow_range', ['range', 'address_and', 'address_or', 'modify_add', 'modify_and', 'modify_or'], (self, range, addressAnd, addressOr, modifyAdd, modifyAnd, modifyOr, context) => {
      var rangeFrom = range.isNil() ? null : range.left;
      var rangeTo = range.isNil() ? null : range.right;

      addressAnd = addressAnd.isNil() ? null : context.asNumber(addressAnd);
      addressOr = addressOr.isNil() ? null : context.asNumber(addressOr);
      modifyAdd = modifyAdd.isNil() ? null : context.asNumber(modifyAdd);
      modifyAnd = modifyAnd.isNil() ? null : context.asNumber(modifyAnd);
      modifyOr = modifyOr.isNil() ? null : context.asNumber(modifyOr);

      self.memory.shadowRange(rangeFrom, rangeTo, addressAnd, addressOr, modifyAdd, modifyAnd, modifyOr);
    })

    klass.on('to_future_number', [], (self, context) => {
      return new FutureNumber(new SymbolLocation(self.getSymbolName()));
    })
  }
}
