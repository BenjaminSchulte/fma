import AbstractObject from './Object';

export default class NilObject extends AbstractObject {
  type() {
    return 'Nil';
  }

  isNil() {
    return true;
  }
}

const Instance = new NilObject;

export {
  Instance
}
