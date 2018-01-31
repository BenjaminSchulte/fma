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
    this.isInQueue = false;
    this.parentLocations = [];
  }

  setParentLocations(locations) {
    this.parentLocations = locations.slice()
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

  processFunctionHooks(callback) {
    var hooks = this.collectFunctionHooks();
    if (!hooks.length) {
      return callback();
    }

    const next = () => {
      if (!hooks.length) {
        return callback();
      }

      hooks.shift().call(this, next);
    }

    next();
  }

  compile(context) {
    if (this.hasBeenCompiled) {
      return;
    }

    this.hasBeenCompiled = true;

    const root = context.getRoot();
    const CompilerScope = (root.resolveChild('CompilerScope')).getObject();
    const Compiler = (root.resolveChild('Compiler')).getObject();
    const scope = CompilerScope.getMember('new').callWithParameters(root, this);

    context.getInterpreter().setCurrentBlock(scope.block);
    Compiler.setMember('current_scope', scope);

    for (let location of this.parentLocations) {
      scope.block.addDependency(location.getFile());
    }

    context.getInterpreter().log('info', `Compiling function ${this.getFullName()}`);
    scope.getMember('on_enter_function').callWithParameters(context, this);

    this.processFunctionHooks(() => {
      if (this.javascriptCallback) {
        this.javascriptCallback(this);
      } else {
        this.parentContext.enter(this).processMany(this.children);
      }
    });

    scope.getMember('on_leave_function').callWithParameters(context, this);
    context.getInterpreter().setCurrentBlock(null);
  }

  callWithParameters(context) {
    const compiler = context.resolveChild('Compiler').getObject();
    if (!this.isInQueue && compiler.hasMember('current_scope')) {
      context.getInterpreter().compileFunction(this);
      this.isInQueue = true;
    }

    return this;
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('to_future_number', [], (self) => {
      return new FutureNumber(new SymbolLocation(self.getSymbolName()));
    })
    klass.on('name', [], (self, context) => {
      return context.returnString(self.getFullName());
    })
  }
}
