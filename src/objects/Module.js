import NamedObject from './NamedObject';

export default class ModuleObject extends NamedObject {
  constructor(name) {
    super(name);

    console.log('NEW MODULE', name);
  }

  type() {
    return 'Module';
  }
}
