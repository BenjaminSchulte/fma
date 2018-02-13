import AbstractLocationHint from './AbstractLocationHint';

export default class LocationHint extends AbstractLocationHint {
  constructor() {
    super();

    this.align = null;
    this.rangeFrom = null;
    this.rangeTo = null;
    this.addressMask = null;
    this.addressInclude = null;
  }

  serialize() {
    return {
      align: this.align,
      rangeFrom: this.rangeFrom,
      rangeTo: this.rangeTo,
      addressMask: this.addressMask,
      addressInclude: this.addressInclude
    }
  }

  applyTo(memory) {
    memory.allowRange(this.rangeFrom, this.rangeTo, this.addressMask, this.addressInclude, this.align);
  }

  allowRange(rangeFrom, rangeTo, addressMask, addressInclude, align) {
    if (rangeFrom !== null) { this.rangeFrom = rangeFrom; }
    if (rangeTo !== null) { this.rangeTo = rangeTo; }
    if (addressMask !== null) { this.addressMask = addressMask; }
    if (addressInclude !== null) { this.addressInclude = addressInclude; }
    if (align !== null) { this.align = align; }
  }
}
