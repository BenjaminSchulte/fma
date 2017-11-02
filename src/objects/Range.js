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

    this.initializeMembers();
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

  initializeMembers() {
    PluginUtils.initializeOnce('range', this, (instance) => {

      PluginUtils.on(instance, 'first', [], async (context) => {
        return await context.create('Number', new InternalValue(this.left));
      })

      PluginUtils.on(instance, 'last', [], async (context) => {
        return await context.create('Number', new InternalValue(this.right));
      })

      PluginUtils.on(instance, 'each', ['&callback'], async (block, context) => {
        var result = [];

        for (var i=this.left; i<=this.right; i++) {
          result.push(await block.getMacro().callWithParameters(context.getContext(), await context.create('Number', new InternalValue(i))));
        }

        return new ArrayObject(result);
      })
    });
  }
}
