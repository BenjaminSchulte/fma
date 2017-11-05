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

    klass.on('register_static_memory', ['memory'], (self, memory) => {
      this.interpreter.addRomBlock(memory.memory);
    })

    klass.on('register_dynamic_memory', ['memory'], (self, memory) => {
      this.interpreter.addRamBlock(memory.memory);
    })

    klass.on('print', ['*args'], (self, args, context) => {
      this.print(context, ...args.getItems());
    })

    klass.on('define', ['&block'], (self, args, context) => {
      const func = new FunctionObject('.codesection' + Compiler.nextCodeSectionId++);

      func.setCallback(() => {
        const block = (context.getContext().resolveChild('block')).getObject();
        block.getMacro().callWithParameters(context.getContext());
      })

      this.interpreter.compileFunction(func);
    })
  }

  print(context, ...messages) {
    var result = [];

    for (let object of messages) {
      result.push(this.asString(context, object));
    }

    console.log(result.join(''));
  }

  asString(context, object) {
    if (object.hasMember('to_s')) {
      const str = object.getMember('to_s').callWithParameters(context.getContext());

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
          items.push(this.asString(context, child));
        }
        return '[' + items.join(', ') + ']'

      case 'Hash':
        var items = [];
        const children = object.getItems();
        for (let key in children) {
          items.push(key + ': ' + (this.asString(context, children[key])));
        }
        return '{' + items.join(', ') + '}'

      case 'Module':
        return '<' + object.type() + ':' + object.getFullName() + '>';

      case 'Range':
        return `${object.getFirstValue()}..${object.getLastValue()}`

      case 'Boolean':
        return object.getValue().toString();

      case 'Class':
        return '<' + object.getClassName() + ':' + object.getName() + '>';
    }

    return "<" + object.getClassName() + ">";
  }
}

Compiler.nextCodeSectionId = 1;
