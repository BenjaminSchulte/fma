import AbstractInterpreter from './AbstractInterpreter';

export default class Comment extends AbstractInterpreter {
  setComments(comments) {
    return false;
  }

  process() {
    if (!this.node.isDocComment()) {
      return;
    }

    this.context.comments.add(this.node);
  }
}
