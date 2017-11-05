export default class MemoryRecalculator {
  constructor(allowed, shadows) {
    this.shadows = shadows;
    this.allowed = allowed;
  }

  getRomAddress(fullAddress) {
    const bank = fullAddress >> 16;
    const address = fullAddress & 0xFFFF;

    for (let shadow of this.shadows) {
      if (shadow.fromBank !== bank) {
        continue;
      }

      if (address < shadow.rangeFrom || address > shadow.rangeTo) {
        continue;
      }

      return ((address + shadow.modifyAdd) & 0xFFFF) | (shadow.toBank << 16);
    }

    return fullAddress;
  }

  getRomOffset(fullAddress) {
    const bank = fullAddress >> 16;
    const address = fullAddress & 0xFFFF;

    for (let allowed of this.allowed) {
      if (allowed.bank !== bank) {
        continue;
      }

      if (address < allowed.rangeFrom || address > allowed.rangeTo) {
        continue;
      }

      return allowed.locatedAt + (address - allowed.rangeFrom);
    }

    throw new Error('ROM address $' + address.toString(16) + ' could not be located.');
  }
}
