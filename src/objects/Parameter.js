import AbstractObject from './Object';
import InterpreterError from '../interpreter/InterpreterError';

export default class Parameter extends AbstractObject {
  constructor(value, type=Parameter.TYPE_ARGUMENT) {
    super();

    this.value = value;
    this.name = null;
    this.parameterType = type;
  }

  setName(name) {
    if (this.parameterType !== Parameter.TYPE_ARGUMENT) {
      throw new InterpreterError('Argument name can not be set on *args, **args or &args');
    }

    this.parameterType = Parameter.TYPE_NAMED_ARGUMENT;
    this.name = name;
  }
}

Parameter.TYPE_ARGUMENT = 'Argument';
Parameter.TYPE_NAMED_ARGUMENT = 'NamedArgument';
Parameter.TYPE_ARGUMENTS = 'Arguments';
Parameter.TYPE_KEYWORD_ARGUMENTS = 'KeywordArguments';
Parameter.TYPE_CONTEXT = 'Context';
Parameter.TYPE_BLOCK = 'Block';
