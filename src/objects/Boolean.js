import AbstractObject from './Object';
import PluginUtils from '../plugin/PluginUtils';

export default class BooleanObject extends AbstractObject {
  constructor(value) {
    super();

    this.value = value;
  }

  isTrue() {
    return this.value;
  }

  getValue() {
    return this.value;
  }

  type() {
    return 'Boolean';
  }

  initializeClassMembers(klass) {
    klass.on('to_b', [], (self) => {
      return self;
    })

    klass.on('&&', ['other'], (self, other, context) => {
      return new BooleanObject(self.value && (context.asBoolean(other)));
    })

    klass.on('||', ['other'], (self, other, context) => {
      return new BooleanObject(self.value || (context.asBoolean(other)));
    })
  }
}
