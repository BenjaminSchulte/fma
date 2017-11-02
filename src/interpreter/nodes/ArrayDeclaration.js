import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import ArrayObject from '../../objects/Array';

export default class ArrayDeclaration extends AbstractInterpreter {
  async process() {
    var items = [];

    for (let item of this.node.getChildren()) {
      const value = await this.context.resolve(item);
      items.push(value.getObject());
    }

    return new ValueAccessor(new ArrayObject(items));
  }
}
