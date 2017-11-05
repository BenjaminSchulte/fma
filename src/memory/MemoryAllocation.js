import OldMemoryAllocation from './old/MemoryAllocation';
import OldMemoryLocation from './old/MemoryLocation';

export default class MemoryAllocation {
  constructor(parent, memory) {
    this.parent = parent;
    this.memory = memory;
    this.children = [];

    this.allowed = [];
    this.shadows = [];

    this.appliedShadows = [];
  }

  createChild() {
    var node = new MemoryAllocation(this, this.memory.allocate());
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

  shadowRange(rangeFrom, rangeTo, addressAnd, addressOr, modifyAdd, modifyAnd, modifyOr) {
    var fromBank = null;
    var toBank = null;

    if (rangeFrom > 0xFFFF || rangeTo > 0xFFFF) {
      throw new Error('Address ranges over 65535 are not supported, yet');
    }

    if (addressAnd === null || addressAnd !== 0xFFFF) {
      throw new Error('Address masks other than 65535 are not supported, yet');
    }

    if (modifyAnd === null || modifyAnd !== 0xFFFF) {
      throw new Error('Address masks other than 65535 are not supported, yet');
    }

    if (addressOr & 0xFF0000 !== addressOr) {
      throw new Error('address_or is only supported for $FF0000, yet');
    }

    fromBank = addressOr >> 16;
    toBank = modifyOr >> 16;

    this.shadows.push({fromBank, rangeFrom, rangeTo, toBank, modifyAdd});
  }

  allowOld(bank, rangeFrom, rangeTo, align) {
    this.allowed.push({bank, rangeFrom, rangeTo, align});
  }

  build() {
    this.buildItem();

    for (let child of this.children) {
      child.build();
    }
  }

  buildItem() {
    for (let allowed of this.allowed) {
      this.buildAllowed(allowed);
    }
  }

  getShadows() {
    var list = this.parent ? this.parent.getShadows() : [];
    return list.concat(this.shadows);
  }

  modifyByShadow(address) {
    for (let shadow of this.getShadows()) {
      if (shadow.fromBank !== address.bank) {
        continue;
      }

      if (address.rangeFrom === null) {
        address.rangeFrom = shadow.rangeFrom;
        address.rangeTo = shadow.rangeTo;
      }

      if (address.rangeFrom < shadow.rangeFrom || address.rangeFrom > shadow.rangeTo) {
        continue;
      }

      this.appliedShadows.push(shadow);
      address = {
        bank: shadow.toBank,
        rangeFrom: (address.rangeFrom + shadow.modifyAdd) & 0xFFFF,
        rangeTo: (address.rangeTo + shadow.modifyAdd) & 0xFFFF,
        align: address.align
      }

      return [address];
    }

    return [address];
  }

  buildAllowed(unmodifiedAllowed) {
    for (let allowed of this.modifyByShadow(unmodifiedAllowed)) {
      var address = OldMemoryLocation.anywhere();

      if (allowed.bank !== null) {
        address = address.inBank(allowed.bank);
      }

      if (allowed.rangeFrom !== null) {
        if (allowed.rangeFrom === allowed.rangeTo) {
          address = address.atAddress(allowed.rangeFrom);
        } else {
          address = address.inRange(allowed.rangeFrom, allowed.rangeTo - allowed.rangeFrom + 1);
        }
      }

      if (allowed.align !== null) {
        address = address.alignedTo(allowed.align);
      }

      this.memory.allow(address);
    }
  }
}
