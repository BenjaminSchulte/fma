export default class NodeSerializer {
  static VERSION = 1;

  constructor() {
    this.strings = [];

    this.locations = [];
    this.locationHashes = {};
  }

  serializeString(string) {
    var index = this.strings.indexOf(string);

    if (index === -1) {
      index = this.strings.length;
      this.strings.push(string);
    }

    return index;
  }

  serializeLocation(location) {
    const file = this.serializeString(location.file);
    const line = location.line;
    const col  = location.column;

    const hash = `${file},${line},${col}`;
    var index;
    if (this.locationHashes.hasOwnProperty(hash)) {
      index = this.locationHashes[hash];
    } else {
      index = this.locations.length;
      this.locations.push([file, line, col]);
      this.locationHashes[hash] = index;
    }

    return index;
  }

  serializeList(nodes) {
    nodes = nodes.filter(node => node.shouldBeSerialized());

    if (!nodes.length) {
      return 0;
    }

    return [
      nodes.map(node => this.serializeString(node.type())),
      nodes.map(node => node.serialize(this))
    ]
  }

  serialize(node) {
    if (node === null) {
      return 0;
    }

    if (!node.shouldBeSerialized()) {
      throw new Error(`Tried to serialize node of type ${node.type()} which should not be serialized`);
    }

    return [this.serializeString(node.type()), node.serialize(this)];
  }

  serializeProgram(program) {
    return this.stringify([
      NodeSerializer.VERSION,
      this.strings,
      this.serialize(program)
    ])
  }

  stringify(tree) {
    return JSON.stringify(tree);
  }
}
