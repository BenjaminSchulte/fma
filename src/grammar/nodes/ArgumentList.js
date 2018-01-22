import ExpressionList from './ExpressionList';

export default class ArgumentList extends ExpressionList {
  static deserialize(s, args) {
    const node = new ArgumentList();
    node.deserialize(s, args);
    return node;
  }
}
