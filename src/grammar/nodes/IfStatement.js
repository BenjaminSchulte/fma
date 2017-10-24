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
}
