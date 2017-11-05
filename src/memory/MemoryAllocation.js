import OldMemoryAllocation from './old/MemoryAllocation';
import OldMemoryLocation from './old/MemoryLocation';

export default class MemoryAllocation {
  constructor(memory) {
    this.memory = memory;
    this.children = [];
  }

  createChild() {
    var node = new MemoryAllocation(this.memory.allocate());
    this.children.push(node);
    return node;
  }

  setName(name) {
    this.memory.withDebugName(name);
  }

  setSize(size) {
    this.memory.size(size);
  }

  allowRange(rangeFrom, rangeTo, addressAnd, addressOr, align) {
    var bank = null;

    if (rangeFrom > 0xFFFF || rangeTo > 0xFFFF) {
      throw new Error('Address ranges over 65535 are not supported, yet');
    }

    if (addressAnd !== null && addressAnd !== 0xFFFF) {
      throw new Error('Address masks other than 65535 are not supported, yet');
    }

    if (addressOr !== null) {
      if (addressOr & 0xFF0000 !== addressOr) {
        throw new Error('address_or is only supported for $FF0000, yet');
      }

      bank = addressOr >> 16;
    }

    this.allowOld(bank, rangeFrom, rangeTo, align);
  }

  allowOld(bank, rangeFrom, rangeTo, align) {
    var address = OldMemoryLocation.anywhere();

    if (bank !== null) {
      address = address.inBank(bank);
    }

    if (rangeFrom !== null) {
      address = address.inRange(rangeFrom, rangeTo - rangeFrom + 1);
    }

    if (align !== null) {
      address = address.alignedTo(align);
    }

    this.memory.allow(address);
  }
}
