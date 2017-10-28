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
}
