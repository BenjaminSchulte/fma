import NamedObject from './NamedObject';

export default class MacroObject extends NamedObject {
  constructor(name) {
    super(name);

    this.javascriptCallback = null;
    this.parentContext = null;
    this.arguments = null;
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

  async call(context) {
    if (this.javascriptCallback) {
      return await this.javascriptCallback(context);
    }

    context.injectParent(this.parentContext);
    context.getObject().setMember('self', this.parentContext.getObject());

    return (await context.processMany(this.children)).getObject();
  }

  async callWithParameters(calleeContext, ...args) {
    const context = this.arguments.buildContext(calleeContext, args, null);

    return await this.call(context);
  }
}
