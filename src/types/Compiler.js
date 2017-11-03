import Class from '../objects/Class';
import FunctionObject from '../objects/Function';
import PluginUtils from '../plugin/PluginUtils';

export default class Compiler extends Class {
  constructor(interpreter) {
    super('Compiler');

    this.interpreter = interpreter;
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass);

    klass.on('register_static_code', ['memory'], async (self, memory) => {
      console.log('TODO: REGISTER STATIC CODE', memory);
    })

    klass.on('print', ['*args'], async (self, args, context) => {
      await this.print(context, ...args.getItems());
    })

    klass.on('define', ['&block'], async (self, args, context) => {
      const func = new FunctionObject('<memorysection>');

      func.setCallback(async () => {
        const block = (await context.getContext().resolveChild('block')).getObject();
        await block.getMacro().callWithParameters(context.getContext());
      })

      this.interpreter.compileFunction(func);
    })

    klass.on('each_function', ['&block'], async (self, block, context) => {
      await block.getMacro().callWithParameters(context.getContext());
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

      if (str.getClassName() === 'String') {
        const member = str.getMember('__value').getValue();
        return member;
      }

      object = str;
    }

    switch (object.getClassName()) {
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

      case 'Module':
        return '<' + object.type() + ':' + object.getFullName() + '>';

      case 'Range':
        return `${object.getFirstValue()}..${object.getLastValue()}`

      case 'Boolean':
        return object.getValue().toString();
    }

    return "<" + object.getClassName() + ">";
  }
}
