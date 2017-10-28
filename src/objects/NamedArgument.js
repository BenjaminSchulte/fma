import NamedObject from './NamedObject';

export default class NamedArgument extends NamedObject {
  constructor(name, value) {
    super(name);

    this.value = value;
  }
}
