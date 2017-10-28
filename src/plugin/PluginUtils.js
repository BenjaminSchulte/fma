export default class PluginUtils {
  constructor(context) {
    this.context = context;
  }

  getContext() {
    return this.context;
  }

  async create(type, ...args) {
    const construct = (await this.context.getRoot().resolveChild('String')).getObject().getMember('new');
    const instance = await construct.callWithParameters(this.context, ...args);
    return instance;
  }

  static onInstance(klass, name, args, callback) {
    klass.onInstance(name, args, async (context) => {
      const self = (await context.resolveChild('self')).getObject();
      var params = [];

      for (let arg of args) {
        const name = arg.match(/^(&|\*|\*\*)?(.*)$/)[2];
        params.push((await context.resolveChild(name)).getObject());
      }

      return await callback(self, ...params, new PluginUtils(context));
    })
  }
}
