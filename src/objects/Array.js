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

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('each', ['&callback'], (self, block, context) => {
      var result = [];

      for (let item of self.items) {
        result.push(block.getMacro().callWithParameters(context.getContext(), item));
      }

      return new ArrayObject(result);
    })
  }
}
