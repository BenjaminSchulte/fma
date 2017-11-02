import ExpressionList from './ExpressionList';

export default class Identifier extends ExpressionList {
  setIsRoot() {
    this.isRoot = true;
  }
}
