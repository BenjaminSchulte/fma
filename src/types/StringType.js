import InternalValueClass from './InternalValueClass';
import PluginUtils from '../plugin/PluginUtils';
import InterpreterError from '../interpreter/InterpreterError';

export default class StringType extends InternalValueClass {
  constructor(parent) {
    super('String', parent);
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'to_s', [], async (self) => {
      return self;
    })

    this.operatorBoolean('==', (a, b) => { return a == b; });
  }
}
