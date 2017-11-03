import Class from '../objects/Class';
import ClassInstance from '../objects/ClassInstance';
import FutureNumber from '../objects/FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import PluginUtils from '../plugin/PluginUtils';

export default class CompilerMemoryManager extends Class {
  constructor(interpreter) {
    super('CompilerMemoryManager');

    this.interpreter = interpreter;
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('allocate', [], async (self, context) => {
      const instance = await context.create('CompilerMemoryManager');

      return instance;
    })

    klass.on('allocate_shadow', [], async (self, context) => {
      const instance = await context.create('CompilerMemoryManager');

      return instance;
    })

    klass.on('to_future_number', [], async (self, context) => {
      return new FutureNumber(new SymbolLocation(self.getFullName()));
    })
  }
}
