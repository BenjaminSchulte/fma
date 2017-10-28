import ObjectClass from './Object';

export default class InternalValue extends ObjectClass {
  constructor(value) {
    super();

    this.value = value;
  }

  getValue() {
    return this.value;
  }

  type() {
    return 'InternalValue';
  }
}
