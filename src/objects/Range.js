import AbstractObject from './Object';

export default class RangeObject extends AbstractObject {
  constructor(left, right) {
    super();

    this.left = left;
    this.right = right;
  }

  getFirstValue() {
    return this.left;
  }

  getLastValue() {
    return this.right;
  }

  type() {
    return 'Range';
  }
}
