import Class from '../objects/Class';
import ClassInstance from '../objects/ClassInstance';
import FutureNumber from '../objects/FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import StaticNumber from '../linker/calculate/StaticNumber';
import Calculation from '../linker/calculate/Calculation';
import PluginUtils from '../plugin/PluginUtils';
import Block from '../objects/Block';
import VariableTypes from '../interpreter/VariableTypes';

import MemoryAllocation from '../memory/MemoryAllocation';
import RootMemoryAllocation from '../memory/RootMemoryAllocation';

export default class CompilerMemoryManager extends Class {
  constructor(interpreter) {
    super('CompilerMemoryManager');

    this.interpreter = interpreter;
  }

  hasInstanceMember(name, self) {
    if (super.hasInstanceMember(name, self)) {
      return true;
    }

    if (self.isArray) {
      return this.hasArrayIndex(name, self);
    }

    if (self.memberKlass && self.instances) {
      return self.instances[0].hasMember(name);
    }

    return false;
  }

  hasArrayIndex(name, self) {
    if (!name.match(/^\d+$/)) {
      return false;
    }

    const index = parseInt(name, 10);
    if (index < 0 || index >= self.memory.getNumItems()) {
      return false;
    }

    return true;
  }

  getInstanceMember(name, self) {
    if (super.hasInstanceMember(name, self)) {
      return super.getInstanceMember(name, self);
    }

    if (self.isArray) {
      const index = parseInt(name, 10);
      if (self.isSystemClass) {
        return new FutureNumber(new Calculation(
          new StaticNumber(index * self.memory.getItemSize()),
          '+',
          new SymbolLocation(self.getSymbolName())
        ));
      } else {
        return self.instances[index];
      }
    }

    return self.instances[0].getMember(name);
  }

  setClassItemType(self, type, context) {
    self.isSystemClass = false;
    self.memberKlass = type;
    self.memberKlassContext = context;
    self.memory.setStringedTogether();

    var times = 1;
    if (self.isArray) {
      times = self.memory.getNumItems();
    }

    self.instances = [];
    while (times-- > 0) {
      self.instances.push(this.createNewInstance(self));
    }
  }

  setItemSize(self, size) {
    self.memory.setItemSize(size);
  }

  setNumItems(self, size) {
    self.isArray = true;
    self.memory.setNumItems(size);

    if (self.memberKlass) {
      while (self.instances.length < size) {
        self.instances.push(this.createNewInstance(self));
      }
    }
  }

  createNewInstance(self) {
    const block = new Block();
    const context = self.memberKlassContext.getContext().enter(block);
    const scope = self.getMember('allocate').callWithParameters(self.memberKlassContext.getContext());
    scope.setNameHint(self.instances.length);
    block.setMember('$scope', scope);

    const instance = self.memberKlass.getMember('new').call(context)
    instance.setMemoryScope(scope);
    instance.setNameParent(scope.getParent());
    instance.setNameHint(self.instances.length);
    return instance;
  }

  setSystemItemType(self, type) {
    self.isSystemClass = true;
    self.memory.setItemSize(VariableTypes.getSizeOf(type));
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

      self.children = [];
      self.isArray = false;
      self.memory = memory;
      self.parent = null;
      this.interpreter.afterwards(() => {
        const name = self.getSymbolName();
        self.memory.setSymbolName(name);
      });
    })

    klass.on('set_item_size', ['size'], (self, size, context) => {
      if (size.isNil()) {
        return;
      }

      this.setItemSize(self, context.asNumber(size));
    });

    klass.on('set_item_type', ['type'], (self, type, context) => {
      if (type.isUndefined()) {
        throw new Error('Item type can not be undefined')
      }
      if (type.isNil()) {
        return;
      }

      if (type.type() === 'Class') {
        this.setClassItemType(self, type, context);
      } else {
        this.setSystemItemType(self, context.asString(type));
      }
    });

    klass.on('set_num_items', ['size'], (self, size, context) => {
      if (size.isNil()) {
        return;
      }

      this.setNumItems(self, context.asNumber(size));
    });

    klass.on('allocate', [], (self, context) => {
      const instance = context.create('CompilerMemoryManager', self);
      self.children.push(instance);
      instance.parent = self;
      return instance;
    })

    klass.on('detach', [], (self, context) => {
      self.memory.detach();
    })

    klass.on('set_is_shared', ['shared'], (self, shared, context) => {
      self.memory.setIsShared(context.asBoolean(shared));
    })

    klass.on('set_dump_usage', ['shared'], (self, shared, context) => {
      self.memory.setDumpUsage(context.asBoolean(shared));
    })

    klass.on('include', ['other'], (self, other, context) => {
      self.memory.addIncluded(other.memory)
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
