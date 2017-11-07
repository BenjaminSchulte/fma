import AbstractObject from './Object';
import PluginUtils from '../plugin/PluginUtils';
import InternalValue from './InternalValue';

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

    klass.on('??', ['other'], (self, other, context) => {
      return self.value ? other : context.create('Number', new InternalValue(0));
    })

    klass.on('||', ['other'], (self, other, context) => {
      return new BooleanObject(self.value || (context.asBoolean(other)));
    })
  }
}
