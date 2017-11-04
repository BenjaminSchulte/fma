import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class MacroPointer extends Class {
  constructor() {
    super('MacroPointer');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('initialize', ['&block'], (self, block, context) => {
      self.setMember('call', block.getMacro());
    })
  }
}
