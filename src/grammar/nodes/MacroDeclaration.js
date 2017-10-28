import BlockDeclaration from './BlockDeclaration';
import ArgumentList from './ArgumentList';

export default class MacroDeclaration extends BlockDeclaration {
  constructor(name) {
    super(name);

    this.arguments = new ArgumentList();
  }

  setArguments(args) {
    this.arguments.setChildren(args);

    return this;
  }

  getArguments() {
    return this.arguments.getChildren();
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeList(this.arguments.getChildren())
    ];
  }
}
