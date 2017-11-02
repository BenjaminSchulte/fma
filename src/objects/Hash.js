import ObjectClass from './Object';
import PluginUtils from '../plugin/PluginUtils';
import InterpreterError from '../interpreter/InterpreterError';
import BooleanObject from './Boolean';

export default class HashObject extends ObjectClass {
  constructor(items={}) {
    super();

    this.items = items;
    this.initializeMembers();
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

  initializeMembers() {
    PluginUtils.initializeOnce('hash', this, () => {
      PluginUtils.on(this, 'key?', ['key'], async (key, context) => {
        if (!key.hasMember('to_s')) {
          throw new InterpreterError(`${key.type()} has no member to_s`);
        }

        const stringObject = await key.getMember('to_s').callWithParameters(context.getContext());
        const str = stringObject.getMember('__value').getValue();
        return new BooleanObject(this.items.hasOwnProperty(str));
      })
    })
  }
}
