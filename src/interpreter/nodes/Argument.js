import AbstractInterpreter from './AbstractInterpreter';
import ArgumentList from '../ArgumentList';
import {ParameterTypes} from '../../grammar/nodes';

export default class Argument extends AbstractInterpreter {
  async process() {

    const name = await this.context.getProcessor(this.node.name).asString();

    var defaultValue = null;
    if (this.node.defaultValue) {
      defaultValue = await this.context.resolve(this.node.defaultValue);
      if (defaultValue.isUndefined()) {
        this.log("warn", "Default value is undefined");
        defaultValue = null;
      } else {
        defaultValue = defaultValue.getObject();
      }
    }

    const list = new ArgumentList();
    list.addArgument(name, this.getArgumentType(), defaultValue);

    return list;

  }

  getArgumentType() {
    switch (this.node.argumentType) {
      case ParameterTypes.ARGUMENT: return ArgumentList.TYPE_ARGUMENT;
      case ParameterTypes.ARGUMENTS: return ArgumentList.TYPE_ARGUMENTS;
      case ParameterTypes.KEYWORD_ARGUMENTS: return ArgumentList.TYPE_KEYWORD_ARGUMENTS;
      case ParameterTypes.BLOCK: return ArgumentList.TYPE_BLOCK;
      default:
        throw new Error('Unhandled argument type');
    }
  }
}
