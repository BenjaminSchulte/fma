import AbstractObject from './Object'

export default class RootObject extends AbstractObject {
  type() {
    return 'Root';
  }

  getName() {
    return '';
  }
}
