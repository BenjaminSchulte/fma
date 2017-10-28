import AbstractObject from './Object';

export default class BooleanObject extends AbstractObject {
  constructor(value) {
    super();

    this.value = value;
  }

  getValue() {
    return this.value;
  }

  type() {
    return 'Boolean';
  }
}
