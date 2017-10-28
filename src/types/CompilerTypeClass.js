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
      if (object.hasMember('to_s')) {
        const str = await object.getMember('to_s').callWithParameters(context.getContext());

        if (str.type() === 'String') {
          const member = str.getMember('__value').getValue();
          result.push(member);
        } else {
          result.push("<???>");
        }
      } else {
        switch (object.type()) {
        case 'Boolean': result.push(object.getValue().toString()); break;
        default: result.push("<" + object.type() + ">"); break;
        }

      }

    }

    console.log(result.join(''));
  }
}
