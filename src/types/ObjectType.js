import Class from '../objects/Class';
import BooleanObject from '../objects/Boolean';
import PluginUtils from '../plugin/PluginUtils';

export default class ObjectType extends Class {
  constructor() {
    super('Object');
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'is_a?', ['type'], async (self, type) => {
      return new BooleanObject(self.klass.getFullName() == type.getFullName());
    })
  }
}
