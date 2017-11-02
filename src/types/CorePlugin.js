import Plugin from '../plugin/Plugin';

import ObjectType from './ObjectType';
import NumberType from './NumberType';
import TypedNumber from './TypedNumber';
import StringType from './StringType';
import MacroPointer from './MacroPointer';

import Compiler from './Compiler';
import CompilerScope from './CompilerScope';
import CompilerMemoryManager from './CompilerMemoryManager';
import ClassInstance from '../objects/ClassInstance';

export default class CorePlugin extends Plugin {
  async register(root, interpreter) {
    root.setMember('Object', new ObjectType());

    root.setMember('Number', new NumberType());
    root.setMember('TypedNumber', new TypedNumber());
    root.setMember('String', new StringType());
    root.setMember('MacroPointer', new MacroPointer());

    root.setMember('Compiler', new ClassInstance(new Compiler(interpreter)));
    root.setMember('CompilerMemoryManager', new CompilerMemoryManager(interpreter));
    root.setMember('CompilerScope', new CompilerScope());
  }
}
