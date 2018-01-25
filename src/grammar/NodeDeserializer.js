import NodeSerializer from './NodeSerializer';
import Location from './Location';
import * as Nodes from './nodes';

export default class NodeDeserializer {
  constructor(strings, locations, locationModifier) {
    this.strings = strings;
    this.locations = locations;
    this.locationModifier = locationModifier;
  }

  deserializeString(index) {
    return this.strings[index];
  }

  deserializeLocation(node) {
    if (node === -1) {
      return null;
    }

    const args = this.locations[node];
    const location = new Location(args[1], args[2]);
    location.file = this.deserializeString(args[0]);

    this.locationModifier(location);

    return location;
  }

  deserializeList(list) {
    if (list === 0) {
      return [];
    }

    const types = list[0].map(item => this.deserializeString(item));

    return list[1].map((item, index) => {
      return this._deserialize(types[index], item);
    });
  }

  deserialize(node) {
    if (node === 0) {
      return null;
    }

    return this._deserialize(this.deserializeString(node[0]), node[1]);
  }

  _deserialize(typeName, node) {
    const Type = Nodes[typeName];

    if (!Type || !Type.deserialize) {
      throw new Error(`Can not deserialize unknown type: ${typeName}`);
    }

    return Type.deserialize(this, node)
  }

  static deserializeProgram(program, locationModifier) {
    if (program[0] !== NodeSerializer.VERSION) {
      throw new Error(`Incompatible version for deserializer. Is ${program[0]}, must be ${NodeSerializer.VERSION}`);
    }

    const instance = new NodeDeserializer(program[1], program[2], locationModifier);
    return instance.deserialize(program[3]);
  }


}
