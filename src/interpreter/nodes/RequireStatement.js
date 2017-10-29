import AbstractInterpreter from './AbstractInterpreter';
import ValueAccessor from '../ValueAccessor';
import Parser from '../../grammar/Parser';
import path from 'path';

export default class RequireStatement extends AbstractInterpreter {
  async process() {

    const result = await this.context.process(this.node.parent);
    const file = await result.getObject().asString(this.context);
    const project = this.context.getInterpreter().getProject();
    const parser = new Parser(project);

    const program = await parser.parseRelativeFile(file + '.pasm', path.dirname(this.node.getLocation().getFile()));

    await this.context.process(program);

  }
}
