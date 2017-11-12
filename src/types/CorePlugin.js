import Plugin from '../plugin/Plugin';

import ObjectType from './ObjectType';
import NumberType from './NumberType';
import TypedNumber from './TypedNumber';
import StringType from './StringType';
import SystemTypeWrapper from './SystemTypeWrapper';
import MacroPointer from './MacroPointer';

import Compiler from './Compiler';
import CompilerScope from './CompilerScope';
import CompilerMemoryManager from './CompilerMemoryManager';
import ClassInstance from '../objects/ClassInstance';
import FutureNumber from '../objects/FutureNumber';
import Class from '../objects/Class';

export default class CorePlugin extends Plugin {
  preProcess(project, interpreter) {
    this.register(interpreter.getRoot(), interpreter);
  }

  register(root, interpreter) {
    root.setMember('Object', new ObjectType());

    root.setMember('FutureNumber', new SystemTypeWrapper(new FutureNumber(null)))
    root.setMember('Class', new SystemTypeWrapper(new Class('$$system')))

    root.setMember('Number', new NumberType());
    root.setMember('TypedNumber', new TypedNumber());
    root.setMember('String', new StringType());
    root.setMember('MacroPointer', new MacroPointer());

    root.setMember('Compiler', new ClassInstance(new Compiler(interpreter)));
    root.setMember('CompilerMemoryManager', new CompilerMemoryManager(interpreter));
    root.setMember('CompilerScope', new CompilerScope());
  }
}
