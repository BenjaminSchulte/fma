import ExpressionNode from './ExpressionNode';
import StatementList from './StatementList';

export default class ExpressionList extends ExpressionNode {
  constructor(name) {
    super(name);

    this.children = new StatementList();
  }

  setChildren(children) {
    for (let child of children) {
      this.addChild(child);
    }

    return this;
  }

  addChild(child) {
    this.children.addChild(child);
    return this;
  }

  getChildren() {
    return this.children.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.getChildren())
    ];
  }
}
