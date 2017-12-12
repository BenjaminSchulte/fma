import Class from '../objects/Class';
import PluginUtils from '../plugin/PluginUtils';

export default class TypedNumber extends Class {
  constructor() {
    super('TypedNumber');
  }

  initializeInstanceMembers(klass) {
    super.initializeInstanceMembers(klass)

    klass.on('initialize', ['number', 'type'], (self, number, type, context) => {
      self.setMember('number', number);
      self.setMember('type', type);
    })

    klass.on('dup', [], (self, context) => {
      const copy = self.klass.getMember('new').callWithParameters(context.getContext(), self.getMember('number'), self.getMember('type'));
      return copy;
    })

    this.operator(klass, '==');
    this.operator(klass, '!=');
    this.operator(klass, '<');
    this.operator(klass, '>');
    this.operator(klass, '+');
    this.operator(klass, '-');
    this.operator(klass, '*');
    this.operator(klass, '/');
    this.operator(klass, '&');
    this.operator(klass, '|');
    this.operator(klass, '<<');
    this.operator(klass, '>>');
  }

  operator(klass, op) {
    klass.on(op, ['other'], (self, other, context) => {
      const copy = self.getMember('dup').callWithParameters(context.getContext());

      const number = copy.getMember('number');
      if (!number.hasMember(op)) {
        throw new Error('Typed number member `number` has no operator: ' + op);
      }

      copy.setMember('number', number.getMember(op).callWithParameters(context.getContext(), other));
      return copy;
    })
  }
}
