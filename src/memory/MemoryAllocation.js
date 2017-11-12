import OldMemoryAllocation from './old/MemoryAllocation';
import OldMemoryLocation from './old/MemoryLocation';
import MemoryAddress from './MemoryAddress';

export default class MemoryAllocation {
  constructor(parent) {
    this.parent = parent;
    this.children = [];

    this.memory = null;

    this.size = null;
    this.numItems = 1;
    this.name = null;
    this.symbolName = null;
    this.isShared = false;

    this.allowed = [];
    this.shadows = [];
    this.included = [];

    this.appliedShadows = [];
    this.isBuild = false;
    this.isAttached = true;
  }

  detach() {
    this.isAttached = false;
  }

  setIsShared(shared) {
    this.isShared = shared;
  }

  addIncluded(other) {
    this.included.push(other);
  }

  getRoot() {
    return this.parent.getRoot();
  }

  getRomOffset(address) {
    return this.getRoot().getRomOffset(address);
  }

  collectSymbols(symbols) {
    if (this.hasMemoryAddress() && this.symbolName) {
      symbols.add(this.symbolName, this.getMemoryAddress().getApplicationAddress());
    }

    for (let child of this.children) {
      child.collectSymbols(symbols);
    }
  }

  hasMemoryAddress() {
    return this.memory.hasStaticAddress();
  }

  getMemoryAddress() {
    if (!this.memory.hasStaticAddress()) {
      throw new Error('No address for memory found');
    }

    return new MemoryAddress(this, this.getAppliedShadows(), this.memory.staticAddress)
  }

  createChild() {
    var node = new MemoryAllocation(this);
    this.children.push(node);
    return node;
  }

  setSymbolName(name) {
    this.symbolName = name;
    this.setName(name);
  }

  setName(name) {
    this.name = name;
  }

  setItemSize(size) {
    this.size = size;
  }

  getItemSize() {
    return this.size;
  }

  setNumItems(num) {
    this.numItems = num;
  }

  getNumItems() {
    return this.numItems;
  }

  getFullSize() {
    if (this.size === null) {
      return this.size;
    }
    return this.size * this.numItems;
  }

  allowRange(rangeFrom, rangeTo, addressAnd, addressOr, align, locatedAt) {
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

    this.allowOld(bank, rangeFrom, rangeTo, align, locatedAt);
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

  allowOld(bank, rangeFrom, rangeTo, align, locatedAt) {
    this.allowed.push({bank, rangeFrom, rangeTo, align, locatedAt});
  }

  buildMemory() {
    const memory = this.parent.memory.allocate();
    if (!this.isAttached) {
      memory.detach();
    }

    memory.size(this.getFullSize()).withDebugName(this.name);
    if (this.isShared) {
      memory.shared();
    }

    for (let included of this.included) {
      included.build();
      memory.link(included.memory);
    }

    this.memory = memory;
  }

  build() {
    if (this.isBuild) {
      return;
    }
    this.isBuild = true;

    this.buildMemory();
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

  getAppliedShadows() {
    var list = this.parent ? this.parent.getAppliedShadows() : [];
    return list.concat(this.appliedShadows);
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
