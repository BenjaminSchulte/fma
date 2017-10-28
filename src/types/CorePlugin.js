import Plugin from '../plugin/Plugin';

import ObjectType from './ObjectType';
import NumberType from './NumberType';
import StringType from './StringType';

import CompilerType from './CompilerType';

export default class CorePlugin extends Plugin {
  async register(root) {
    root.setMember('Object', new ObjectType());

    root.setMember('Number', new NumberType(root.getMember('Object')));
    root.setMember('String', new StringType(root.getMember('Object')));

    root.setMember('Compiler', new CompilerType());
  }
}
