import NamedObject from './NamedObject';

export default class ModuleObject extends NamedObject {
  constructor(name) {
    super(name);
  }

  type() {
    return 'Module';
  }
}
