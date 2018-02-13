export default class WriterSymbol {
  constructor(offset, name) {
    this.offset = offset;
    this.name = name;
  }

  serialize() {
    return {
      offset: this.offset,
      name: this.name
    }
  }
}
