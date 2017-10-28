import ObjectClass from './Object';

export default class HashObject extends ObjectClass {
  constructor(items={}) {
    super();

    this.items = items;
  }

  getItems() {
    return this.items;
  }

  type() {
    return 'Hash';
  }
}
