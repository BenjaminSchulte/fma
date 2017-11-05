export default class BufferSizeModifier {
  resize(buffer, headerLocation) {
    const targetBufferSize = this.getTargetBufferSize(buffer.length);
    const newSize = BufferSizeModifier.BUFFER_SIZE[targetBufferSize];

    var newBuffer = new Uint8Array(newSize);
    newBuffer.set(buffer, 0)

    newBuffer[headerLocation + 0x27] = targetBufferSize;

    return new Buffer(newBuffer);
  }

  getTargetBufferSize(size) {
    for (let value in BufferSizeModifier.BUFFER_SIZE) {
      if (size <= BufferSizeModifier.BUFFER_SIZE[value]) {
        return value;
      }
    }

    throw new Error('ROM file is too large');
  }
}

BufferSizeModifier.BUFFER_SIZE = {
  0x08: 2 * 1024 * 1024 / 8,
  0x09: 4 * 1024 * 1024 / 8,
  0x0A: 8 * 1024 * 1024 / 8,
  0x0B: 16 * 1024 * 1024 / 8,
  0x0C: 32 * 1024 * 1024 / 8,
  0x0D: 64 * 1024 * 1024 / 8
};
