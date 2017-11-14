export default class DynamicBuffer {
  constructor(buffer=null, stepSize=1024, emptyMemory=false) {
    this.stepSize = stepSize;
    this.emptyMemory = emptyMemory;

    if (buffer) {
      this.buffer = buffer;
      this.offset = 0;
      this.size = buffer.length;
      this.totalSize = buffer.length;
    } else {
      this.size = 0;
      this.totalSize = this.stepSize;
      this.offset = 0;
      this.buffer = new Buffer(this.totalSize);
      this.cleanUp(0, this.totalSize);
    }
  }

  getLength() {
    return this.size;
  }

  getBuffer() {
    return this.buffer.slice(0, this.size);
  }

  resize(size) {
    const newBuffer = new Buffer(size);
    this.buffer.copy(newBuffer);
    this.buffer = newBuffer;
    this.cleanUp(this.size, size);
    this.totalSize = size;
  }

  cleanUp(from, to) {
    if (!this.emptyMemory) {
      return;
    }

    while (from < to) {
      this.buffer[from++] = 0;
    }
  }

  allocate(size) {
    const newSize = size + this.size;

    if (newSize > this.totalSize) {
      this.resize((newSize + this.stepSize) & ~(this.stepSize - 1));
    }

    this.size += size;
  }

  writeTo(buffer, offset) {
    const size = buffer.length;
    const endOfData = offset + size;

    if (endOfData > this.totalSize) {
      this.resize((endOfData + this.stepSize) & ~(this.stepSize - 1));
    }

    this.size = Math.max(this.size, endOfData);
    buffer.copy(this.buffer, offset, 0, size);
  }

  write(buffer, size=null, offset=0) {
    if (size === null) {
      size = buffer.length;
    }

    this.allocate(size);

    buffer.copy(this.buffer, this.offset, offset, offset + size);
    this.offset += size;
  }

  writeUInt8(number) {
    this.allocate(1);
    this.buffer[this.offset++] = number;
  }

  writeInt8(number) {
    this.allocate(1);

    if (number < 0) {
      number = (~number & 0xFF);
    }

    this.buffer[this.offset++] = number;
  }

  writeUInt16LE(number) {
    this.allocate(2);
    this.buffer[this.offset++] = number & 0xFF;
    this.buffer[this.offset++] = number >> 8;
  }

  writeUInt16BE(number) {
    this.allocate(2);
    this.buffer[this.offset++] = number >> 8;
    this.buffer[this.offset++] = number & 0xFF;
  }
}
