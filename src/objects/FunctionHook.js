import Macro from './Macro';
import ArgumentList from '../interpreter/ArgumentList';
import NilObject from './Nil';

export default class FunctionHook {
  constructor(macro, context, self) {
    this.macro = macro;
    this.context = context;
    this.self = self;
  }

  call(self, callback) {
    const macro = new Macro('decorator' + FunctionHook.nextId++);
    macro.setCallback(() => {
      callback();
      return new NilObject();
    });
    macro.setArguments(new ArgumentList());

    const context = this.context.clone();
    context.getObject().setMember('yield', macro);

    this.macro.callWithoutDecoratorCheck(context, this.self);
  }
}

FunctionHook.nextId = 1;
