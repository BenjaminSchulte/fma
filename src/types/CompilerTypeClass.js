import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class CompilerTypeClass extends Class {
  constructor() {
    super('Compiler');
  }

  initializeMembers() {
    super.initializeMembers();

    PluginUtils.onInstance(this, 'print', ['*args'], async (self, args, context) => {
      await this.print(context, ...args.getItems());
    })
  }

  async print(context, ...messages) {
    var result = [];

    for (let object of messages) {
      result.push(await this.asString(context, object));
    }

    console.log(result.join(''));
  }

  async asString(context, object) {
    if (object.hasMember('to_s')) {
      const str = await object.getMember('to_s').callWithParameters(context.getContext());

      if (str.type() === 'String') {
        const member = str.getMember('__value').getValue();
        return member;
      }

      object = str;
    }

    switch (object.type()) {
      case 'Array':
        var items = [];
        for (let child of object.getItems()) {
          items.push(await this.asString(context, child));
        }
        return '[' + items.join(', ') + ']'

      case 'Hash':
        var items = [];
        const children = object.getItems();
        for (let key in children) {
          items.push(key + ': ' + (await this.asString(context, children[key])));
        }
        return '{' + items.join(', ') + '}'

      case 'Range':
        return `${object.getFirstValue()}..${object.getLastValue()}`

      case 'Boolean':
        return object.getValue().toString();
    }

    return "<" + object.type() + ">";
  }
}
