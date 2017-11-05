import Bank from './section/Bank';

export default class StaticAddress {
  constructor(section, offset, size) {
    this.section = section;
    this.offset = offset;
    this.size = size;
  }

  clone() {
    return new StaticAddress(this.section, this.offset, this.size);
  }

  asReadAddress() {
    var bank = 0;
    if (this.section instanceof Bank) {
      bank = this.section.address;
    }

    return (bank << 16) | this.offset;
  }

  asWriteAddress() {
    return this.asReadAddress();
  }
}
