import AbstractObject from './Object';
import PluginUtils from '../plugin/PluginUtils';

export default class BooleanObject extends AbstractObject {
  constructor(value) {
    super();

    this.value = value;
    this.initializeMembers();
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

  initializeMembers() {
    PluginUtils.initializeOnce('boolean', this, (instance) => {

      instance.on('to_b', [], async () => {
        return this;
      })

      instance.on('&&', ['other'], async (context) => {
        const other = await context.resolveChild('other');

        return new BooleanObject(this.value && (await other.asBoolean(context)));
      })

    });
  }
}
