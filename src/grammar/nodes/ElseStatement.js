import StatementList from './StatementList';

export default class ElseStatement extends StatementList {
  static deserialize(s, args) {
    const node = new ElseStatement();
    node.deserialize(s, args);
    return node;
  }
}
