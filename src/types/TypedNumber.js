import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class TypedNumber extends Class {
  constructor() {
    super('TypedNumber');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass)

    klass.on('initialize', ['number', 'type'], async (self, number, type, context) => {
      self.setMember('number', number);
      self.setMember('type', type);
    })
  }
}
