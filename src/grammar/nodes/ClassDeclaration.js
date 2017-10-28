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
}
