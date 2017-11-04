import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import Hash from '../../objects/Hash';

export default class HashDeclaration extends AbstractInterpreter {
  process() {
    var hash = {};

    for (let item of this.node.getItems()) {
      const name = this.context.getProcessor(item.name).asString();
      const value = this.context.resolve(item.value);

      hash[name] = value.getObject();
    }

    return new ValueAccessor(new Hash(hash));
  }
}
