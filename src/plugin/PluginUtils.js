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

  async asString(self, log=null) {
    return await this.asType(self, log, 'to_s', 'ClassInstance', "String", "", (object) => {
      return object.getMember('__value').getValue();
    });
  }

  async asBoolean(self, log=null) {
    if (typeof(self) !== 'object') {
      const BooleanObject = PluginUtils.require('../objects/Boolean');
      return new BooleanObject(!!self);
    }

    return await this.asType(self, log, 'to_b', 'Boolean', "", "", (object) => {
      return object.getValue()
    });
  }

  async asType(object, log, methodName, resultType, resultClassType, result, callback) {
    if (!log) {
      const interpreter = this.context.getInterpreter();
      log = interpreter.log.bind(interpreter);
    }

    if (object.isUndefined()) {
      log("warn", "Expression result is Undefined");
    } else {
      if (!object.hasMember(methodName)) {
        log("warn", `Can not convert object of type ${object.type()} to ${resultType}`);
      } else {
        const targetObject = await object.getMember(methodName).callWithParameters(this.context);
        var isValid = true;

        if (targetObject.type() !== resultType) {
          isValid = false;
        } else if (resultClassType && targetObject.getClassName() !== resultClassType) {
          isValid = false;
        }

        if (!isValid) {
          log("error", `${object.getName()}.${methodName} returns no ${resultType} object`);
        } else {
          result = callback(targetObject);
        }
      }
    }

    return result;
  }
/*
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
  }*/
/*
  static asBoolean(value) {
    const BooleanObject = require('../objects/Boolean').default;
    return new BooleanObject(value);
  }*/

  static createMacro(self, name, args, callback) {
    const ArgumentList = this.require('../interpreter/ArgumentList');
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = this.require('../objects/Macro');
    const macro = new Macro(name);
    macro.setCallback(async (context) => {
      var params = [];

      for (let arg of args) {
        const name = arg.match(/^(&|\*{1,2})?(.*)$/)[2];
        params.push((await context.resolveChild(name)).getObject());
      }

      return await callback(self, ...params, new PluginUtils(context));
    });
    macro.setArguments(list);

    return macro;
  }

/*
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
  */

  static require(name) {
    if (!PluginUtils.instances[name]) {
      PluginUtils.instances[name] = require(name).default;
    }

    return PluginUtils.instances[name];
  }
}


PluginUtils.instances = {}
