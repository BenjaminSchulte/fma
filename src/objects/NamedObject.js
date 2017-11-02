import AbstractObject from './Object';

export default class NamedObject extends AbstractObject {
  constructor(name) {
    super();

    if (!name)  {
      throw new Error('Missing argument name')
    }

    this.name = name;
  }

  getName() {
    return this.name;
  }

  setMember(name, object) {
    super.setMember(name, object);
  }
}
