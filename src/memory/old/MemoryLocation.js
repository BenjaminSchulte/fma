import Utils from './utils';

export default class MemoryLocation {
  constructor() {
    this.banks = [];
    this.addresses = [];
    this.addressRanges = [];
    this.alignedToAddresses = [];
    this.sections = [];
    this.data = {}
  }

  clone() {
    const location = new MemoryLocation;

    location.banks = this.banks.slice();
    location.addresses = this.addresses.slice();
    location.addressRanges = this.addressRanges.slice();
    location.alignedToAddresses = this.alignedToAddresses.slice();
    location.sections = this.sections.slice();
    location.data = this.data;

    return location;
  }

  merged(other) {
    const location = new MemoryLocation;

    location.addresses = other.addresses.slice();
    location.alignedToAddresses = other.alignedToAddresses.slice();
    location.sections = Utils.arrayUnique(this.sections.concat(other.sections));
    location.data = Object.assign({}, this.data, other.data);

    location.banks = [];
    if (this.banks.length && !other.banks.length) {
      location.banks = this.banks.slice();
    } else if (other.banks.length && !this.banks.length) {
      location.banks = other.banks.slice();
    } else if (other.banks.length && this.banks.length) {
      location.banks = Utils.arrayIntersect(this.banks, other.banks);

      if (!location.banks.length) {
        return null;
      }
    }

    location.addressRanges = [];
    if (this.addressRanges.length && !other.addressRanges.length) {
      location.addressRanges = this.addressRanges.slice();
    } else if (other.addressRanges.length && !this.addressRanges.length) {
      location.addressRanges = other.addressRanges.slice();
    } else if (other.addressRanges.length && this.addressRanges.length) {
      for (let left of this.addressRanges) {
        for (let right of other.addressRanges) {
          const start = Math.max(left[0], right[0]);
          const end   = Math.min(left[0] + left[1], right[0] + right[1]);

          if (start >= end) {
            continue;
          }

          location.addressRanges.push([start, end - start]);
        }
      }

      if (!location.addressRanges.length) {
        return null;
      }
    }

    return location;
  }

  advance(step) {
    const location = this.clone();

    location.addresses = this.addresses.map(address => address + step);
    location.alignedToAddresses = [];

    return location;
  }

  isStaticAddress() {
    return this.addresses.length;
  }

  isStaticBank() {
    return this.banks.length;
  }

  isAlignedAddress() {
    return this.alignedToAddresses.length;
  }

  getBanks() {
    return this.banks;
  }

  ratePriority(baseScore=0) {
    var staticBank = 0;
    var staticAddress = 0;
    var addressRanges = 0;
    var alignedAddress = 0;

    if (this.banks.length) {
      staticBank = 10 - Math.min(9, this.banks.length);
    }
    if (this.addresses.length) {
      staticAddress = 10 - Math.min(9, this.addresses.length);
    }
    if (this.addressRanges.length) {
      addressRanges = 10 - Math.min(9, this.addressRanges.length);
    }
    if (this.alignedToAddresses.length) {
      alignedAddress = Math.max.apply(Math, this.alignedToAddresses);
    }
    if (this.sections.length) {
      console.log(this.sections);
      throw new Error('TODO');
    }

    var score = baseScore;
    score = (score * 10) + staticAddress;
    score = (score * 10) + staticBank;
    score = (score * 10) + addressRanges;
    score = (score * 0x10000) + alignedAddress;
    return score;
  }

  flags() {
    var flags = 0;

    flags |= this.banks.length ? MemoryLocation.FLAG_STATIC_BANK : 0;
    flags |= this.addresses.length ? MemoryLocation.FLAG_STATIC_ADDRESS : 0;
    flags |= this.addressRanges.length ? MemoryLocation.FLAG_ADDRESS_RANGE : 0;
    flags |= this.alignedToAddresses.length ? MemoryLocation.FLAG_ALIGNED_ADDRESS : 0;
    flags |= this.sections.length ? MemoryLocation.FLAG_STATIC_SECTION : 0;

    return flags;
  }

  at(bank, address) {
    return this.inBank(bank).atAddress(address);
  }

  alignedTo(address) {
    this.alignedToAddresses.push(address);
    return this;
  }

  atAddress(address) {
    this.addresses.push(address);
    return this;
  }

  inRange(offset, size) {
    this.addressRanges.push([offset, size]);
    return this;
  }

  inBank(bank) {
    if (bank === null) {
      return this;
    }

    this.banks.push(bank);
    return this;
  }

  inSection(section) {
    this.sections.push(section);
    return this;
  }

  toString() {
    const banks = this.banks.map(value => '$'+value.toString(16)).join(',');
    const addresses = this.addresses.map(value => '$'+value.toString(16)).join(',');
    const alignedToAddresses = this.alignedToAddresses.map(value => '$'+value.toString(16)).join(',');
    const addressRanges = this.addressRanges.map(value => '$'+value[0].toString(16)+'-$'+(value[0]+value[1]-1).toString(16)).join(',');
    return `MemoryLocation<BANK:${banks} ADDRESS:${addresses} RANGE:${addressRanges} ALIGN:${alignedToAddresses}>`
  }

  static anywhere() {
    return new MemoryLocation();
  }
}

MemoryLocation.FLAG_STATIC_BANK     = 0x01;
MemoryLocation.FLAG_STATIC_SECTION  = 0x02;
MemoryLocation.FLAG_STATIC_ADDRESS  = 0x04;
MemoryLocation.FLAG_ALIGNED_ADDRESS = 0x08;
MemoryLocation.FLAG_ADDRESS_RANGE   = 0x10;

const STATICS = [
  "at",
  "alignedTo",
  "atAddress",
  "inBank",
  "inRange",
  "inSection"
]

for (let staticName of STATICS) {
  MemoryLocation[staticName] = function(...args) {
    const location = MemoryLocation.anywhere();
    return location[staticName](...args);
  }
}
