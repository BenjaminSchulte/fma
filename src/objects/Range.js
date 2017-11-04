import AbstractObject from './Object';
import PluginUtils from '../plugin/PluginUtils';
import InternalValue from './InternalValue';
import InterpreterError from '../interpreter/InterpreterError';
import ArrayObject from './Array';

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

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('first', [], (self, context) => {
      return context.create('Number', new InternalValue(self.left));
    })

    klass.on('last', [], (self, context) => {
      return context.create('Number', new InternalValue(self.right));
    })

    klass.on('each', ['&callback'], (self, block, context) => {
      var result = [];

      for (var i=self.left; i<=self.right; i++) {
        result.push(block.getMacro().callWithParameters(context.getContext(), context.create('Number', new InternalValue(i))));
      }

      return new ArrayObject(result);
    })
  }
}
