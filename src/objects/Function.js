import NamedObject from './NamedObject';
import FutureNumber from './FutureNumber';
import SymbolLocation from '../linker/calculate/SymbolLocation';
import PluginUtils from '../plugin/PluginUtils';

export default class FunctionObject extends NamedObject {
  constructor(name) {
    super(name);

    this.hasBeenCompiled = false;
    this.javascriptCallback = null;
    this.children = null;
  }

  setCallback(javascriptCallback) {
    this.javascriptCallback = javascriptCallback;
  }

  setChildren(children) {
    this.children = children;
  }

  setParentContext(context) {
    this.parentContext = context;
  }

  type() {
    return 'Function';
  }

  canBeCalled() {
    return true;
  }

  compile(context) {
    if (this.hasBeenCompiled) {
      return;
    }

    this.hasBeenCompiled = true;

    const root = context.getRoot();
    const CompilerScope = (root.resolveChild('CompilerScope')).getObject();
    const Compiler = (root.resolveChild('Compiler')).getObject();
    const scope = CompilerScope.getMember('new').callWithParameters(root);

    Compiler.setMember('current_scope', scope);

    context.getInterpreter().log('info', `Compiling function ${this.name}`);
    scope.getMember('on_enter_function').callWithParameters(context, this);

    if (this.javascriptCallback) {
      this.javascriptCallback(this);
    } else {
      this.parentContext.processMany(this.children);
    }

    scope.getMember('on_leave_function').callWithParameters(context, this);
    context.getInterpreter().log('info', `Finished compiling function ${this.name}`);

    //context.processMany(this.children);
  }

  callWithParameters(context) {
    context.getInterpreter().compileFunction(this);

    return this;
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('to_future_number', [], (self) => {
      return new FutureNumber(new SymbolLocation(self.getFullName()));
    })
  }
}
