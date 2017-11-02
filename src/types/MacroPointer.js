import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class MacroPointer extends Class {
  constructor() {
    super('MacroPointer');
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'initialize', ['&block'], async (self, block, context) => {
      console.log('MacroPointer block', block);
    })
  }
}
