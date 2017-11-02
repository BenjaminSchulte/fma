export default class ValueAccessor {
  constructor(child) {
    this.child = child;
  }

  isUndefined() {
    return this.child.type() === 'Undefined';
  }

  getObject() {
    return this.child;
  }

  getObjectName() {
    if (this.child.name) {
      return this.child.name;
    }

    return this.getObjectType();
  }

  getObjectType() {
    return this.child.type();
  }

  async asString(context, log=null) {
    return await this.asType(context, log, 'to_s', 'String', "", (object) => {
      return object.getMember('__value').getValue();
    });
  }

  async asBoolean(context, log=null) {
    return await this.asType(context, log, 'to_b', 'Boolean', "", (object) => {
      return object.getValue()
    });
  }

  async asType(context, log, methodName, resultType, result, callback) {
    if (!log) {
      const interpreter = context.getInterpreter();
      log = interpreter.log.bind(interpreter);
    }

    if (this.isUndefined()) {
      log("warn", "Expression result is Undefined");
    } else {
      const object = this.getObject();

      if (!object.hasMember(methodName)) {
        log("warn", `Can not convert object of type ${object.type()} to ${resultType}`);
      } else {
        const targetObject = await object.getMember(methodName).callWithParameters(context);
        if (targetObject.type() !== resultType) {
          log("error", `${object.type()}.${methodName} returns no ${resultType} object`);
        } else {
          result = callback(targetObject);
        }
      }
    }

    return result;
  }

}
