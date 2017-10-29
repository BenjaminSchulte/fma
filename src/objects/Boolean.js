import AbstractObject from './Object';

export default class BooleanObject extends AbstractObject {
  constructor(value) {
    super();

    this.value = value;
  }

  isTrue() {
    return this.value;
  }

  getValue() {
    return this.value;
  }

  type() {
    return 'Boolean';
  }
}
