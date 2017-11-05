import Writer from './writer/Writer';
import LocationHint from './location/LocationHint';

export default class StaticCodeBlock {
  constructor() {
    this.code = new Writer();
    this.locationHint = new LocationHint();
  }

  getSize() {
    return this.code.size();
  }

  getLocationHint() {
    return this.locationHint;
  }

  registerInMemory(memory) {
    const child = memory.createChild();
    child.setSize(this.getSize());
    child.setName(this.code.getFirstSymbol());

    this.locationHint.applyTo(child);

    this.location = child;
  }
}
