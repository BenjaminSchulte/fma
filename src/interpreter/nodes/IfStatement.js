import AbstractInterpreter from './AbstractInterpreter';

export default class IfStatement extends AbstractInterpreter {
  async process() {

    const condition = await this.context.resolve(this.node.condition);
    var result = false;
    if (condition.isUndefined()) {
      this.log("warn", "Expression result is Undefined");
    } else {
      const object = condition.getObject();

      if (!object.hasMember('to_b')) {
        this.log("warn", `Can not convert object ob type ${object.type()} to boolean`);
      } else {
        const boolObject = await object.getMember('to_b').callWithParameters(this.context);
        if (boolObject.type() !== 'Boolean') {
          this.log("error", `${object.type()}.to_b returns no Boolean object`);
        } else {
          result = boolObject.getValue();
        }
      }
    }

    if (result) {
      return await this.context.processMany(this.node.getChildren());
    } else if (this.node.otherwise.type() === 'ElseStatement') {
      return await this.context.processMany(this.node.otherwise.getChildren());
    } else {
      throw new Error('TODO');
    }

  }
}
