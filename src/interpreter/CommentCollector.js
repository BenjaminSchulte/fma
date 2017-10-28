export default class CommentCollector {
  constructor() {
    this.reset();
  }

  reset() {
    this.comments = [];
    this.lastLine = null;
  }

  getLastLine() {
    return this.lastLine;
  }

  hasComments() {
    return this.comments.length;
  }

  getComments() {
    return this.comments.map(comment => comment.getDocComment()).join("\n");
  }

  add(comment) {
    this.comments.push(comment);

    this.lastLine = comment.getLocation().getLine();
  }
}
