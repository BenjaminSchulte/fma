import NamedObject from './NamedObject';

export default class ModuleObject extends NamedObject {
  constructor(name) {
    super(name);
  }

  type() {
    return 'Module';
  }

  setMember(name, value) {
    if (value.type() === 'Macro') {
    //  value = value.withSelf(value);
    }

    super.setMember(name, value);
  }
}
