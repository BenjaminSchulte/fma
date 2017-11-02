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
}
