export default class DynamicBuffer {
  constructor(buffer=null) {
    if (buffer) {
      this.buffer = buffer;
      this.offset = 0;
      this.size = buffer.length;
      this.totalSize = buffer.length;
    } else {
      this.size = 0;
      this.totalSize = 1024;
      this.offset = 0;
      this.buffer = new Buffer(this.totalSize);
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
    this.totalSize = size;
  }

  allocate(size) {
    const newSize = size + this.size;

    if (newSize > this.totalSize) {
      this.resize((this.totalSize + 1024) & ~1023);
    }

    this.size += size;
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
