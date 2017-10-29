import AbstractObject from './Object';

export default class NilObject extends AbstractObject {
  type() {
    return 'Nil';
  }
}
