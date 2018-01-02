import Context from './Context';
import Block from '../objects/Block';
import ReadOnlyBlock from '../objects/ReadOnlyBlock';
import Parameter from '../objects/Parameter';
import ArrayObject from '../objects/Array';
import MacroPointer from '../objects/MacroPointer';
import HashObject from '../objects/Hash';
import UndefinedObject from '../objects/Undefined';
import {Instance as NilObject} from '../objects/Nil';
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
    var defaultValue = null;

    for (let item of list) {
      if (item[0] === '?') {
        item = item.substr(1);
        defaultValue = NilObject;
      }

      if (item[0] === '*') {
        if (item[1] === '*') {
          this.addArgument(item.substr(2), ArgumentList.TYPE_KEYWORD_ARGUMENTS, defaultValue);
        } else {
          this.addArgument(item.substr(1), ArgumentList.TYPE_ARGUMENTS, defaultValue);
        }
      } else if (item[0] === '&') {
        this.addArgument(item.substr(1), ArgumentList.TYPE_BLOCK, defaultValue);
      } else {
        this.addArgument(item, ArgumentList.TYPE_ARGUMENT, defaultValue);
      }
    }
  }

  buildContext(calleeContext, params, parent = null) {
    const block = new Block();
    var context = new Context(calleeContext.getInterpreter(), block, parent);
    context.stack = calleeContext.stack;
    context.locationStack = calleeContext.locationStack;

    var args = [];
    var kwargs = {};
    var blockObject = null;
    var readOnlyBlocks = [];

    for (let param of params) {
      if (param.type() === 'Parameter') {
        switch (param.parameterType) {
          case Parameter.TYPE_ARGUMENT:
            args.push(param.value);
            break;

          case Parameter.TYPE_NAMED_ARGUMENT:
            kwargs[param.name] = param.value;
            break;

          case Parameter.TYPE_BLOCK:
            if (!param.value.isNil()) {
              if (param.value.type() !== 'MacroPointer') {
                throw new InterpreterError('Parameter must be macro pointer, but is ' + param.value.type());
              }
              blockObject = param.value;
            }
            break;

          case Parameter.TYPE_CONTEXT:
            var newBlock = new ReadOnlyBlock(param.value);
            readOnlyBlocks.push(newBlock);
            context = new Context(calleeContext.getInterpreter(), newBlock, parent);
            context = context.injectParent(new Context(calleeContext.getInterpreter(), block));
            break;

          case Parameter.TYPE_ARGUMENTS:
            args = args.concat(param.value.getItems());
            break;

          case Parameter.TYPE_KEYWORD_ARGUMENTS:
            const items = param.value.getItems();
            for (let key in items) {
              kwargs[key] = items[key];
            }
            break;

          default:
            throw new InterpreterError('Parameter type not supported, yet: ' + param.parameterType);
        }
      } else {
        args.push(param);
      }
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
            block.setMember(arg.name, arg.defaultValue);
          } else {
            console.log(arg);
            throw new InterpreterError(`Missing argument for macro: ${arg.name}`)
          }
          break;
      }
    }

    if (argumentsName) {
      block.setMember(argumentsName, new ArrayObject(args));
      args = [];
    }

    if (kwArgumentsName) {
      block.setMember(kwArgumentsName, new HashObject(kwargs));
      kwargs = {};
    }

    if (blockName) {
      if (!blockObject) {
        blockObject = NilObject;
      }

      block.setMember(blockName, blockObject);
    } else if (blockObject) {
      block.setMember('yield', blockObject.getMacro());
    }

    block.setMember('callee', calleeContext.getObject());

    if (args.length) {
      throw new InterpreterError('Too many arguments for method');
    }

    if (Object.keys(kwargs).length) {
      throw new InterpreterError('Too many arguments for method: ' + Object.keys(kwargs).join(', '));
    }

    for (let ro of readOnlyBlocks) {
      ro.lock();
    }

    return context;
  }

  buildContextByProxy(context) {
    const args = (context.resolveChild('args')).getObject().getItems();
    const kwargs = (context.resolveChild('kwargs')).getObject().getItems();
    const block = (context.resolveChild('block')).getObject();

    var params = [...args];
    for (let key in kwargs) {
      const param = new Parameter(kwargs[key]);
      param.setName(key);
      params.push(param);
    }

    if (block.type() !== 'Undefined') {
      params.push(new Parameter(block, Parameter.TYPE_BLOCK));
    }

    const calleeContext = new Context(context.getInterpreter(), (context.resolveChild('callee')).getObject());
    const callContext = this.buildContext(calleeContext, params);

    return callContext;
  }
}

ArgumentList.TYPE_ARGUMENT = 'Argument';
ArgumentList.TYPE_ARGUMENTS = 'Arguments';
ArgumentList.TYPE_KEYWORD_ARGUMENTS = 'KeywordArguments';
ArgumentList.TYPE_BLOCK = 'Block';
