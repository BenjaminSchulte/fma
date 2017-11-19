import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import path from 'path';

export default class RequireStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.parent);
    const file = result.getObject().asString(this.context);
    const project = this.context.getInterpreter().getProject();

    var program = this.context.getInterpreter().preprocessedRequire.parseRelativeFile(file + '.fma', path.dirname(this.node.getLocation().getFile()));

    this.context.process(program);

  }
}
