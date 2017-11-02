export default class PluginUtils {
  constructor(context) {
    this.context = context;
  }

  getContext() {
    return this.context;
  }

  async create(type, ...args) {
    const construct = (await this.context.getRoot().resolveChild(type)).getObject().getMember('new');
    const instance = await construct.callWithParameters(this.context, ...args);
    return instance;
  }

  static initializeOnce(name, target, callback) {
    var members = {};
    const memberName = `__instance_members_${name}`;
    const oldMembers = Object.keys(target.getMembers());

    if (target[memberName]) {
      members = target[memberName];
      for (let member in members) {
        target.setMember(member, members[member]);
      }
      return;
    }

    callback(target);

    const newMembers = target.getMembers();
    for (let member in newMembers) {
      if (oldMembers.indexOf(member) >= 0) {
        continue;
      }

      members[member] = newMembers[member];
    }

    target[memberName] = members;
  }

  static asBoolean(value) {
    const BooleanObject = require('../objects/Boolean').default;
    return new BooleanObject(value);
  }

  static onInstance(klass, name, args, callback) {
    klass.onInstance(name, args, async (context) => {
      const self = (await context.resolveChild('self')).getObject();
      var params = [];

      for (let arg of args) {
        const name = arg.match(/^(&|\*{1,2})?(.*)$/)[2];
        params.push((await context.resolveChild(name)).getObject());
      }

      return await callback(self, ...params, new PluginUtils(context));
    })
  }

  static on(klass, name, args, callback) {
    klass.on(name, args, async (context) => {
      var params = [];

      for (let arg of args) {
        const name = arg.match(/^(&|\*{1,2})?(.*)$/)[2];
        params.push((await context.resolveChild(name)).getObject());
      }

      return await callback(...params, new PluginUtils(context));
    })
  }
}
