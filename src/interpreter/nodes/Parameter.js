import AbstractInterpreter from './AbstractInterpreter';
import ParameterObject from '../../objects/Parameter';
import ValueAccessor from '../ValueAccessor';
import MacroPointer from '../../objects/MacroPointer';
import {ParameterTypes} from '../../grammar/nodes';

export default class Parameter extends AbstractInterpreter {
  process() {
    var value;

    if (this.node.parameterType === ParameterTypes.BLOCK) {
      value = this.context.process(this.node.value);
      if (value.getObjectType() === 'Macro') {
        value = new ValueAccessor(new MacroPointer(value.getObject()));
      } else if (value.getObjectType() !== 'MacroPointer') {
        throw new Error('Block parameter must be MacroPointer, but is: ' + value.getObjectType());
      }
    } else {
      value = this.context.resolve(this.node.value);
    }

    const parameter = new ParameterObject(value.getObject(), this.getParameterType())

    if (this.node.name) {
      const name = this.context.getProcessor(this.node.name).asString();
      parameter.setName(name);
    }

    return new ValueAccessor(parameter);

  }

  getParameterType() {
    switch (this.node.parameterType) {
      case ParameterTypes.ARGUMENT: return ParameterObject.TYPE_ARGUMENT;
      case ParameterTypes.ARGUMENTS: return ParameterObject.TYPE_ARGUMENTS;
      case ParameterTypes.KEYWORD_ARGUMENTS: return ParameterObject.TYPE_KEYWORD_ARGUMENTS;
      case ParameterTypes.CONTEXT: return ParameterObject.TYPE_CONTEXT;
      case ParameterTypes.BLOCK: return ParameterObject.TYPE_BLOCK;
      default:
        throw new Error(`Unknown parameter type: ${this.node.parameterType}`);
    }
  }
}
