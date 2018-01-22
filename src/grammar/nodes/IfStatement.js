import StatementList from './StatementList';

export default class IfStatement extends StatementList {
  constructor() {
    super();

    this.condition = null;
    this.otherwise = null;
  }

  setCondition(condition) {
    this.condition = condition;

    return this;
  }

  setElse(otherwise) {
    this.otherwise = otherwise;

    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.condition),
      s.serialize(this.otherwise)
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.condition = s.deserialize(args[1]);
    this.otherwise = s.deserialize(args[2]);
  }

  static deserialize(s, args) {
    const node = new IfStatement();
    node.deserialize(s, args);
    return node;
  }
}
