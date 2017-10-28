import AbstractObject from './Object';

export default class NamedObject extends AbstractObject {
  constructor(name) {
    super();

    this.name = name;
  }

  setMember(name, object) {
    console.log("SET", this.name + '.' + name)
    super.setMember(name, object);
  }
}
