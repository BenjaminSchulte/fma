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

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.value = s.deserialize(args[1]);
  }

  static deserialize(s, args) {
    const node = new SwitchCase();
    node.deserialize(s, args);
    return node;
  }
}
