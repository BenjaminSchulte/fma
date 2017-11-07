import NamedObject from '../objects/NamedObject';

export default class SystemTypeWrapper extends NamedObject {
  constructor(klass) {
    super(klass.getClassName())

    this.klass = klass;
  }

  setInstanceMember(name, member) {
    this.klass.klassMembers.on(name, ['*args', '**kwargs', '&block'], (self, args, kwargs, block, context) => {
      return member.call(member.getArguments().buildContextByProxy(context.getContext()), self);
    })
  }

  type() {
    return 'Class';
  }
}
