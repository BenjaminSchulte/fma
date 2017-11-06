export default class PluginUtils {
  constructor(context) {
    this.context = context;
  }

  getInterpreter() {
    return this.context.getInterpreter();
  }

  getContext() {
    return this.context;
  }

  create(type, ...args) {
    const construct = (this.context.getRoot().resolveChild(type)).getObject().getMember('new');
    const instance = construct.callWithParameters(this.context, ...args);
    return instance;
  }

  asString(self, log=null) {
    return this.asType(self, log, 'to_s', 'ClassInstance', "String", "", (object) => {
      return object.getMember('__value').getValue();
    });
  }

  asNumber(self, log=null) {
    return this.asType(self, log, 'to_n', 'ClassInstance', "Number", "", (object) => {
      return object.getMember('__value').getValue();
    });
  }

  asBoolean(self, log=null) {
    if (typeof(self) !== 'object') {
      const BooleanObject = PluginUtils.require('../objects/Boolean');
      return new BooleanObject(!!self);
    }

    return this.asType(self, log, 'to_b', 'Boolean', "", "", (object) => {
      return object.getValue()
    });
  }

  asType(object, log, methodName, resultType, resultClassType, result, callback) {
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
        const targetObject = object.getMember(methodName).callWithParameters(this.context);
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

  static createMacro(name, args, callback) {
    const ArgumentList = this.require('../interpreter/ArgumentList');
    const list = new ArgumentList();
    list.buildFromStringList(args);

    const Macro = this.require('../objects/Macro');
    const macro = new Macro(name);
    macro.setCallback((context, self) => {
      var params = [];

      for (let arg of args) {
        const name = arg.match(/^\??(&|\*{1,2})?(.*)$/)[2];
        params.push((context.resolveChild(name)).getObject());
      }

      return callback(self, ...params, new PluginUtils(context));
    });
    macro.setArguments(list);

    return macro;
  }

  static require(name) {
    if (!PluginUtils.instances[name]) {
      PluginUtils.instances[name] = require(name).default;
    }

    return PluginUtils.instances[name];
  }
}


PluginUtils.instances = {}
