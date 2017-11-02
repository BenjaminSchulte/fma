import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class TypedNumber extends Class {
  constructor() {
    super('TypedNumber');
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'initialize', ['number', 'type'], async (self, number, type, context) => {
      self.setMember('number', number);
      self.setMember('type', type);
    })
  }
}
