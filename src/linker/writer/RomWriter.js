import DynamicBuffer from './DynamicBuffer';

export default class RomWriter {
  constructor() {
    this.buffer = new DynamicBuffer(null, 0x10000, true);
  }

  add(block) {
    const size   = block.getSize();
    const offset = block.getRomOffset();

    this.buffer.writeTo(block.getBuffer().getBuffer(), offset);
  }

  get() {
    return this.buffer;
  }
}
