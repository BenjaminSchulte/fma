import StatementList from './StatementList';

export default class SwitchCase extends StatementList {
  constructor() {
    super();

    this.value = null;
  }

  setValue(value) {
    this.value = value;

    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serialize(this.value)
    ];
  }
}
