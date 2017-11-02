import Class from '../objects/Class';
import ClassInstance from '../objects/ClassInstance';
import FutureNumber from '../objects/FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import PluginUtils from '../plugin/PluginUtils';

export default class CompilerMemoryManager extends Class {
  constructor(interpreter) {
    super('CompilerMemoryManager');

    this.interpreter = interpreter;

    PluginUtils.onInstance(this, 'allocate', [], async (self, context) => {
      const instance = await context.create('CompilerMemoryManager');

      return instance;
    })

    PluginUtils.onInstance(this, 'allocate_shadow', [], async (self, context) => {
      const instance = await context.create('CompilerMemoryManager');

      return instance;
    })

    PluginUtils.onInstance(this, 'to_future_number', [], async (self, context) => {
      return new FutureNumber(new SymbolLocation(self.getFullName()));
    })
  }

  initializeMembers() {
    super.initializeMembers();
  }
}
