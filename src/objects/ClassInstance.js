import ObjectClass from './Object';
import Context from '../interpreter/Context';
import ArgumentList from '../interpreter/ArgumentList'

export default class ClassInstanceObject extends ObjectClass {
  constructor(klass) {
    super();

    this.klass = klass;
  }

  type() {
    return this.klass.name;
  }

  hasMember(name) {
    if (super.hasMember(name)) {
      return true;
    }

    return this.klass.hasInstanceMember(name);
  }

  getMember(name) {
    if (super.hasMember(name)) {
      return super.getMember(name);
    }

    const member = this.klass.getInstanceMember(name);
    if (member.type() === 'Macro') {
      return this.getInstancedMember(member);
    }

    return member;
  }

  getInstancedMember(member) {
    const list = new ArgumentList();
    list.buildFromStringList(['*args', '**kwargs', '&block'])

    const Macro = require('./Macro').default;
    const macro = new Macro();
    macro.setArguments(list);
    macro.setCallback(async (context) => {
      const callContext = await member.getArguments().buildContextByProxy(context);
      callContext.getObject().setMember('self', this);

      return await member.call(callContext);
    });
    return macro;
  }
}
