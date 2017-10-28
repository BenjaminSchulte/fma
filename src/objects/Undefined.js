import AbstractObject from './Object';
import BooleanObject from './Boolean';

export default class UndefinedObject extends AbstractObject {
  constructor() {
    super();

    this.initializeMembers();
  }

  type() {
    return 'Undefined';
  }

  initializeMembers() {
    this.on('==', ['other'], async (context) => {
      const result = (await context.resolveChild('other')).isUndefined();
      return new BooleanObject(result);
    })
  }
}
