import MemorySection from './MemorySection';

export default class Bank extends MemorySection {
  constructor(address, size=0x10000, offset=0) {
    super(offset, size);

    this.address = address;
  }

  isBank(indices) {
    return indices.indexOf(this.address) >= 0;
  }
}
