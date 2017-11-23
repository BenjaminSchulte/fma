import NamedObject from './NamedObject';
import MacroInstance from './MacroInstance';
import FunctionHook from './FunctionHook';

export default class MacroObject extends NamedObject {
  constructor(name, location) {
    super(name);

    this.javascriptCallback = null;
    this.parentContext = null;
    this.arguments = null;
    this.isDecorator = false;
    this.locations = [];
  }

  setLocations(locations) {
    this.locations = locations;
  }

  getLocations() {
    return this.locations;
  }

  setIsDecorator(decorator) {
    this.isDecorator = decorator;
  }

  withSelf(self) {
    const instance = new MacroInstance(this);
    instance.self = self;
    return instance;
  }

  setArguments(arg) {
    this.arguments = arg;
  }

  getArguments() {
    return this.arguments;
  }

  setChildren(children) {
    this.children = children;
  }

  setParentContext(context) {
    this.parentContext = context;
  }

  setCallback(callback) {
    this.javascriptCallback = callback;
  }

  type() {
    return 'Macro';
  }

  canBeCalled() {
    return true;
  }

  collectFunctionHooks() {
    return this.functionHooks;
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

  call(context, self=null) {
    if (this.isDecorator) {
      context.resolveChild('callee').getObject().addFunctionHook(
        new FunctionHook(this, context, self)
      );
      return;
    }

    return this.callWithoutDecoratorCheck(context, self);
  }

  callWithoutDecoratorCheck(context, self) {
    try {
      var result = null;

      this.processFunctionHooks(() => {
        if (this.javascriptCallback) {
          result = this.javascriptCallback(context, self);
          return;
        }

        context = context.injectParent(this.parentContext);

        if (!self) {
          self = context.resolveChild('self').getObject();
          if (self.isUndefined()) {
            self = this.parentContext.getObject();
          }
        }

        if (self) {
          context.getObject().setMember('self', self);
        }

        context.preprocessMany(this.children);

        const block = context.getInterpreter().getCurrentBlock();
        if (block) {
          for (let location of this.getLocations()) {
            block.addDependency(location.getFile());
          }
        }

        result = (context.processMany(this.children)).getObject();
      })

      return result;
    } catch(err) {
      if (err.isReturn) {
        return err.returnValue;
      } else {
        throw err;
      }
    }
  }

  callWithParameters(calleeContext, ...args) {
    const context = this.arguments.buildContext(calleeContext, args, null);

    return this.call(context);
  }
}
