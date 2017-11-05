export default class MemoryAddress {
  constructor(node, shadows, staticAddress) {
    this.node = node;
    this.shadows = shadows;
    this.staticAddress = staticAddress;
  }

  getApplicationAddress() {
    const fullAddress = this.getActualAddress();
    const address = fullAddress & 0xFFFF;
    const bank = (fullAddress >> 16) & 0xFF;

    for (let shadow of this.shadows) {
      if (bank !== shadow.toBank) {
        continue;
      }

      const rangeLeft = (shadow.rangeFrom + shadow.modifyAdd) & 0xFFFF;
      const rangeRight = (shadow.rangeTo + shadow.modifyAdd) & 0xFFFF;
      if (address < rangeLeft || address >= rangeRight) {
        continue;
      }

      return ((address - shadow.modifyAdd) & 0xFFFF) | (shadow.fromBank << 16);
    }

    return fullAddress;
  }

  getActualAddress() {
    return this.staticAddress.asReadAddress();
  }

  getRomOffset() {
    const address = this.getActualAddress();

    return this.node.getRomOffset(address);
  }
}
