import StatementList from './StatementList';

export default class SwitchStatement extends StatementList {
  constructor() {
    super();

    this.condition = null;
  }

  setCondition(condition) {
    this.condition = condition;

    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.condition)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.condition = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new SwitchStatement();
    node.deserialize(s, args);
    return node;
  }
}
