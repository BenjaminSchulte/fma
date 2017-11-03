import ClonedObject from './ClonedObject'

export default class MacroInstance extends ClonedObject {
  constructor(macro) {
    super(macro);

    this.self = null;
  }

  getArguments() {
    return this.object.getArguments();
  }

  canBeCalled() {
    return true;
  }

  async callWithParameters(calleeContext, ...args) {
    const context = this.getArguments().buildContext(calleeContext, args, null);

    return await this.call(context);
  }

  call(context) {
    return this.object.call(context, this.self);
  }
}
