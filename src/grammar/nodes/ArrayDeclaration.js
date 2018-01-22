import ExpressionList from './ExpressionList';

export default class ArrayDeclaration extends ExpressionList {
  static deserialize(s, args) {
    const node = new ArrayDeclaration();
    node.deserialize(s, args);
    return node;
  }
}
