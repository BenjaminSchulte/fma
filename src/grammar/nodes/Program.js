import StatementList from './StatementList';

export default class Program extends StatementList {
  static deserialize(s, args) {
    const node = new Program();
    node.deserialize(s, args);
    return node;
  }
}
