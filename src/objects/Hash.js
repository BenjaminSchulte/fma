import ObjectClass from './Object';
import PluginUtils from '../plugin/PluginUtils';
import InterpreterError from '../interpreter/InterpreterError';
import InternalValue from './InternalValue';
import BooleanObject from './Boolean';
import ArrayObject from './Array';

export default class HashObject extends ObjectClass {
  constructor(items={}) {
    super();

    this.items = items;
    this.membersCanBeSet = true;
  }

  getItems() {
    return this.items;
  }

  type() {
    return 'Hash';
  }

  hasMember(member) {
    if (this.items.hasOwnProperty(member)) {
      return true;
    }

    return super.hasMember(member);
  }

  getMember(member) {
    if (this.items.hasOwnProperty(member)) {
      return this.items[member];
    }

    return super.getMember(member);
  }

  setMember(member, value) {
    if (this.membersCanBeSet) {
      this.items[member] = value;
      return;
    }

    super.setMember(member, value);
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('keys', [], (self, context) => {
      return new ArrayObject(Object.keys(self.items).map(key => {
        return context.create('String', new InternalValue(key));
      }))
    });

    klass.on('key?', ['key'], (self, key, context) => {
      if (!key.hasMember('to_s')) {
        throw new InterpreterError(`${key.type()} has no member to_s`);
      }

      const stringObject = key.getMember('to_s').callWithParameters(context.getContext());
      const str = stringObject.getMember('__value').getValue();
      return new BooleanObject(self.items.hasOwnProperty(str));
    });
  }
}
