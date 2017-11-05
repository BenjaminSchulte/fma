import MemoryAllocation from './MemoryAllocation';
import MemoryArranger from './old/arrange/MemoryArranger';
import Memory from './old/Memory';

export default class RootMemoryAllocation extends MemoryAllocation {
  constructor() {
    super(null);
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

  hasMemoryAddress() {
    return false;
  }

  merge(other) {
    throw new Error('merging memory allocations is not supported, yet');
  }

  buildMemory() {
    this.memory =  new Memory();
  }

  build() {
    super.build();

    const arranger = new MemoryArranger(this.memory);
    arranger.arrange();
  }
}
