import NamedObject from './NamedObject';
import MacroInstance from './MacroInstance';
import FunctionHook from './FunctionHook';

export default class MacroObject extends NamedObject {
  constructor(name) {
    super(name);

    this.javascriptCallback = null;
    this.parentContext = null;
    this.arguments = null;
    this.isDecorator = false;
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
      if (this.javascriptCallback) {
        return this.javascriptCallback(context, self);
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

      return (context.processMany(this.children)).getObject();
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
