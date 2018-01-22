import Declaration from './Declaration';

export default class LabelDeclaration extends Declaration {
  static deserialize(s, args) {
    const node = new LabelDeclaration();
    node.deserialize(s, args);
    return node;
  }
}
