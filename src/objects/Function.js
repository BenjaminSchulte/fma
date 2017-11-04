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

  async compile(context) {
    if (this.hasBeenCompiled) {
      return;
    }

    this.hasBeenCompiled = true;

    const root = context.getRoot();
    const CompilerScope = (await root.resolveChild('CompilerScope')).getObject();
    const Compiler = (await root.resolveChild('Compiler')).getObject();
    const scope = await CompilerScope.getMember('new').callWithParameters(root);

    Compiler.setMember('current_scope', scope);

    context.getInterpreter().log('info', `Compiling function ${this.name}`);
    await scope.getMember('on_enter_function').callWithParameters(context, this);

    if (this.javascriptCallback) {
      await this.javascriptCallback(this);
    } else {
      await this.parentContext.processMany(this.children);
    }

    await scope.getMember('on_leave_function').callWithParameters(context, this);
    context.getInterpreter().log('info', `Finished compiling function ${this.name}`);

    //context.processMany(this.children);
  }

  async callWithParameters(context) {
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
