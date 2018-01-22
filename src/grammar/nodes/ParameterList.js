import ExpressionList from './ExpressionList';

export default class ParameterList extends ExpressionList {
  static deserialize(s, args) {
    const node = new ParameterList();
    node.deserialize(s, args);
    return node;
  }
}
