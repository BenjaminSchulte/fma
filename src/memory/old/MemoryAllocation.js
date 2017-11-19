import MemoryAllocationContainer from './MemoryAllocationContainer';
import MemoryLocation from './MemoryLocation';

export default class MemoryAllocation extends MemoryAllocationContainer {
  constructor(sections, parent) {
    super(sections.createChild());

    this.parents = (parent === null) ? [] : [parent];

    this._size = null;
    this._shared = false;
    this._allow = [];
    this._name = null;

    this.stringedTogether = false;
    this.staticAddress = null;
    this.sharedStaticAddressAllocations = [];
  }

  isStringedTogether() {
    if (this.stringedTogether) {
      return true;
    }

    for (let parent of this.parents) {
      if (parent.isStringedTogether()) {
        return true;
      }
    }

    return false;
  }

  withStringedTogether() {
    this.stringedTogether = true;
    return this;
  }

  withDebugName(name) {
    this._name = name;
    return this;
  }

  isLinked() {
    return this.parents.length > 1;
  }

  link(allocation) {
    allocation.parents.push(this);
    this.children.push(allocation);

    this.sections.attach(allocation.sections);
  }

  detach() {
    for (let parent of this.parents) {
      parent.detachChild(this);
    }

    this.parents = [];
    this.sections.detach();

    return this;
  }

  hasStaticAddress() {
    return this.staticAddress !== null;
  }

  setStaticAddress(address) {
    this.staticAddress = address.clone();

    for (let linkedAllocation of this.sharedStaticAddressAllocations) {
      linkedAllocation.sections.allocateSimilar(address);
    }
  }

  getTotalSize() {
    if (this._size) {
      return this._size;
    }

    var sizes = this.children.map(child => child.getTotalSize());
    var size;
    if (!sizes.length) {
      size = 0;
    } else if (this._shared) {
      size = Math.max(...sizes);
    } else {
      size = sizes.reduce((a, b) => { return a+b; }, 0)
    }

    return size;
  }

  isShared() {
    return this._shared;
  }

  getOwnSectionSize() {
    if (this.isStringedTogether()) {
      return this.getTotalSize();
    }

    return this._size || 0;
  }

  getAllowed() {
    if (this._allow.length) {
      return this._allow;
    }

    return [MemoryLocation.anywhere()];
  }

  setAllowed(list) {
    this._allow = list.slice();
  }

  size(size=null) {
    this._size = size;
    return this;
  }

  allow(location) {
    this._allow.push(location);
    return this;
  }

  deny(location) {
    throw new Error('deny is not implemented, yet')
  }

  shared(shared=true) {
    if (this._shared === shared) {
      return this;
    }

    this._shared = shared;
    this.sections.setIsShared(true);

    return this;
  }

  dump(spaces="") {
    const attr = (key, value=true) => {
      var color = "33";

      if (value === null) {
        value = "NIL";
        color = "31";
      } else {
        switch (typeof value) {
          case 'boolean':
            color = value ? "32;1" : "31;1";
            break;

          case 'number':
          case 'string':
            break;

          default:
            value = "?";
            color = "41;37;1";
            break;
        }
      }

      return "\x1b[32m" + key + ":\x1b[" + color + "m" + value.toString() + "\x1b[m";
    }

    var flags = [];
    if (this._name) { flags.push(attr('NAME', this._name)); }
    if (this._size) { flags.push(attr('SIZE', this._size)); }
    if (this._shared) { flags.push(attr('SHARED')); }

    var allow = [];

    if (this.staticAddress) {
      var allowFlags = [];
      allowFlags.push(attr('FROM', this.staticAddress.section.address.toString(16) + ":" + this.staticAddress.offset.toString(16)));
      allowFlags.push(attr('TO', this.staticAddress.section.address.toString(16) + ":" + (this.staticAddress.offset + this.staticAddress.size - 1).toString(16)));
      allow.push(`\x1b[32;1m== \x1b[m<${allowFlags.join(' ')}>`);
    }

    for (let location of this.getAllowed()) {
      var allowFlags = [];
      allowFlags.push(attr('BANK', (location.banks.length ? location.banks.map(bank => bank.toString(16)).join(',') : null)));
      allowFlags.push(attr('ADDRESS', (location.addresses.length ? location.addresses.map(address => address.toString(16)).join(',') : null)));
      allowFlags.push(attr('RANGE', (location.addressRanges.length ? location.addressRanges.map(range => `${range[0].toString(16)}-${(range[0]+range[1]-1).toString(16)}`).join(',') : null)));
      allowFlags.push(attr('ALIGN', (location.alignedToAddresses.length ? location.alignedToAddresses.map(address => address.toString(16)).join(',') : null)));

      allow.push(`\x1b[34;1mLOC\x1b[m<${allowFlags.join(' ')}>`);
    }

    var prefix = `\x1b[41;37;1m N \x1b[m`;
    if (this.staticAddress) {
      var prefix = `\x1b[42;37;1m Y \x1b[m`;
    }

    console.log(`${spaces}${prefix} ${this.sections.id} \x1b[34;1mALLOCATION\x1b[m<${flags.join(' ')}> @ ${allow.join(' || ')}`);

    for (let child of this.children) {
      child.dump(spaces + '  ')
    }
  }
}
