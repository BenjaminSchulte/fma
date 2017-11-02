import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import Hash from '../../objects/Hash';

export default class HashDeclaration extends AbstractInterpreter {
  async process() {
    var hash = {};

    for (let item of this.node.getItems()) {
      const name = await this.context.getProcessor(item.name).asString();
      const value = await this.context.resolve(item.value);

      hash[name] = value.getObject();
    }

    return new ValueAccessor(new Hash(hash));
  }
}
