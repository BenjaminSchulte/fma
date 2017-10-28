import Context from './Context';
import Block from '../objects/Block';
import ArrayObject from '../objects/Array';
import HashObject from '../objects/Hash';
import UndefinedObject from '../objects/Undefined';
import InterpreterError from './InterpreterError';

export default class ArgumentList {
  constructor() {
    this.arguments = [];
  }

  addArgument(name, type=ArgumentList.TYPE_ARGUMENT, defaultValue=null) {
    this.arguments.push({name, type, defaultValue});
  }

  mergeArguments(other) {
    this.arguments = this.arguments.concat(other.arguments);
  }

  buildFromStringList(list) {
    for (let item of list) {
      if (item[0] === '*') {
        if (item[1] === '*') {
          this.addArgument(item.substr(2), ArgumentList.TYPE_KEYWORD_ARGUMENTS);
        } else {
          this.addArgument(item.substr(1), ArgumentList.TYPE_ARGUMENTS);
        }
      } else if (item[0] === '&') {
        this.addArgument(item.substr(1), ArgumentList.TYPE_BLOCK);
      } else {
        this.addArgument(item);
      }
    }
  }

  buildContext(calleeContext, params, parent = null) {
    const block = new Block();
    const context = new Context(calleeContext.getInterpreter(), block, parent);

    var args = [];
    var kwargs = {};
    var blockObject = null;

    for (let param of params) {
      args.push(param);
    }

    var argumentsName = null;
    var kwArgumentsName = null;
    var blockName = null;
    for (let arg of this.arguments) {
      switch (arg.type) {
        case ArgumentList.TYPE_ARGUMENTS:
          argumentsName = arg.name;
          break;

        case ArgumentList.TYPE_KEYWORD_ARGUMENTS:
          kwArgumentsName = arg.name;
          break;

        case ArgumentList.TYPE_BLOCK:
          blockName = arg.name;
          break;

        case ArgumentList.TYPE_ARGUMENT:
          if (kwargs.hasOwnProperty(arg.name)) {
            block.setMember(arg.name, kwargs[arg.name]);
            delete kwargs[arg.name];
          } else if (args.length) {
            block.setMember(arg.name, args.shift());
          } else if (arg.defaultValue) {
            //context.setMember(arg.name, )
            throw new InterpreterError('TODO: default value');
          } else {
            throw new InterpreterError(`Missing argument for macro: ${arg.name}`)
          }
          break;
      }
    }

    if (argumentsName) {
      block.setMember(argumentsName, new ArrayObject(args));
    }

    if (kwArgumentsName) {
      block.setMember(kwArgumentsName, new HashObject(kwargs));
    }

    if (blockName) {
      if (!blockObject) {
        blockObject = new UndefinedObject();
      }

      block.setMember(blockName, blockObject);
    }

    block.setMember('callee', calleeContext.getObject());

    return context;
  }

  async buildContextByProxy(context) {
    const args = (await context.resolveChild('args')).getObject().getItems();
    const kwargs = (await context.resolveChild('kwargs')).getObject().getItems();
    const block = (await context.resolveChild('block')).getObject();

    var params = [...args];
    for (let key in kwargs) {
      throw new InterpreterError('TODO: pass keywords');
    }

    if (block.type() !== 'Undefined') {
      params.push(block);
    }

    const calleeContext = new Context(context.getInterpreter(), (await context.resolveChild('callee')).getObject());
    const callContext = this.buildContext(calleeContext, params);

    return callContext;
  }
}

ArgumentList.TYPE_ARGUMENT = 'Argument';
ArgumentList.TYPE_ARGUMENTS = 'Arguments';
ArgumentList.TYPE_KEYWORD_ARGUMENTS = 'KeywordArguments';
ArgumentList.TYPE_BLOCK = 'Block';
