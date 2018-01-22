import BlockDeclaration from './BlockDeclaration';

export default class ModuleDeclaration extends BlockDeclaration {
  static deserialize(s, args) {
    const node = new ModuleDeclaration();
    node.deserialize(s, args);
    return node;
  }
}
