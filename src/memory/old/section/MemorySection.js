export default class MemorySection {
  constructor(offset, size) {
    this.offset = offset;
    this.size = size;
  }

  isBank(indices) {
    return false;
  }
}
