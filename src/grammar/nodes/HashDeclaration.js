import ExpressionNode from './ExpressionNode';

export default class HashDeclaration extends ExpressionNode {
  constructor() {
    super();

    this.items = [];
  }

  setItems(items) {
    this.items = items;
    return this;
  }

  getItems() {
    return this.items;
  }

  serialize(s) {
    return [
      super.serialize(s),
      this.items.map(item => {
        return [
          s.serialize(item.name),
          s.serialize(item.value)
        ]
      })
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.items = args[1].map(item => {
      return {
        name: s.deserialize(item[0]),
        value: s.deserialize(item[1])
      }
    })
  }

  static deserialize(s, args) {
    const node = new HashDeclaration();
    node.deserialize(s, args);
    return node;
  }
}
