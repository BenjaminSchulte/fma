import ObjectClass from './Object';

export default class ArrayObject extends ObjectClass {
  constructor(items=[]) {
    super();

    this.items = items;
  }

  getItems() {
    return this.items;
  }

  type() {
    return 'Array';
  }
}
