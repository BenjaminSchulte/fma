import Class from '../objects/Class';
import BooleanObject from '../objects/Boolean';
import PluginUtils from '../plugin/PluginUtils';

export default class ObjectType extends Class {
  constructor() {
    super('Object');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('is_a?', ['type'], async (self, type) => {
      console.log('IS_A on ObjectType')
      return new BooleanObject(self.klass.getFullName() == type.getFullName());
    })
  }
}
