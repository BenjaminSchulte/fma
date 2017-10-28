import Node from './Node';

export default class Comment extends Node {
  constructor(text) {
    super();

    this.text = text;
  }

  isDocComment() {
    return this.text.length && this.text[0] === ';';
  }

  getDocComment() {
    return this.text.substr(1);
  }

  shouldBeSerialized() {
    return false;
  }
}
