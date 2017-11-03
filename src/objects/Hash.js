import ObjectClass from './Object';
import PluginUtils from '../plugin/PluginUtils';
import InterpreterError from '../interpreter/InterpreterError';
import BooleanObject from './Boolean';

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

    klass.on('key?', ['key'], async (self, key, context) => {
      if (!key.hasMember('to_s')) {
        throw new InterpreterError(`${key.type()} has no member to_s`);
      }

      const stringObject = await key.getMember('to_s').callWithParameters(context.getContext());
      const str = stringObject.getMember('__value').getValue();
      return new BooleanObject(self.items.hasOwnProperty(str));
    });
  }
}
