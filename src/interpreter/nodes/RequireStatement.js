import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import Parser from '../../grammar/Parser';
import path from 'path';

export default class RequireStatement extends AbstractInterpreter {
  process() {

    const result = this.context.process(this.node.parent);
    const file = result.getObject().asString(this.context);
    const project = this.context.getInterpreter().getProject();
    const parser = new Parser(project);

    const program = parser.parseRelativeFile(file + '.pasm', path.dirname(this.node.getLocation().getFile()));

    this.context.process(program);

  }
}
