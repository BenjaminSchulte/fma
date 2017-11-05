import MemoryAllocation from './MemoryAllocation';
import Memory from './old/Memory';

export default class RootMemoryAllocation extends MemoryAllocation {
  constructor() {
    super(null, new Memory());
  }

  buildAllowed(allowed) {
    if (allowed.align !== null) {
      throw new Error('Align is not supported for root memory, yet');
    }

    if (allowed.rangeFrom !== 0) {
      throw new Error('Root memory range must start at address 0');
    }

    this.memory.registerBank(allowed.bank, allowed.rangeTo + 1);
  }

  merge(other) {
    throw new Error('merging memory allocations is not supported, yet');
  }
}
