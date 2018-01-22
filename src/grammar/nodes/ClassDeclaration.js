import BlockDeclaration from './BlockDeclaration';
import StatementList from './StatementList';

export default class ClassDeclaration extends BlockDeclaration {
  constructor(name) {
    super(name);

    this.extendedClasses = new StatementList();
  }

  setExtends(ext) {
    this.extendedClasses.setChildren(ext);

    return this;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.extendedClasses.getChildren())
    ];
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.extendedClasses.setChildren(s.deserializeList(args[1]));
  }

  static deserialize(s, args) {
    const node = new ClassDeclaration(null);
    node.deserialize(s, args);
    return node;
  }
}
