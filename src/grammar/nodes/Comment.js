import Node from './Node';

export default class Comment extends Node {
  constructor(text) {
    super();

    console.log('Comment: ', text);
  }
}
