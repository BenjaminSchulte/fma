export default class ChecksumCalculator {
  calculate(buffer, headerLocation) {
    var checksum = this.calculateChecksum(buffer);

    buffer[headerLocation + 0x2E] = (checksum) & 0xFF;
    buffer[headerLocation + 0x2F] = (checksum >> 8) & 0xFF;
    checksum ^= 0xFFFF;
    buffer[headerLocation + 0x2C] = (checksum) & 0xFF;
    buffer[headerLocation + 0x2D] = (checksum >> 8) & 0xFF;
  }

  calculateChecksumMirror(buffer, start, size, mask) {
    while (!(size & mask)) {
      if (mask === 0) {
        throw new Error('Fatal error')
      }
      mask >>= 1;
    }

    var part1 = this.calculateChecksumDefault(buffer, start, mask)
    var part2 = 0;

    var nextLength = size - mask;
    if (nextLength) {
	    part2 = this.calculateChecksumDefault(buffer, start + mask, nextLength, mask >> 1);

  		while (nextLength < mask) {
  			nextLength += nextLength;
  			part2 += part2;
  		}
    }

    return (part1 + part2) & 0xFFFF;
  }

  calculateChecksumDefault(buffer, start, size) {
    var sum = 0;
    for (var i=0; i<size; i++) {
      sum = (sum + buffer[start++]) & 0xFFFF;
    }
    return sum;
  }

  calculateChecksum(buffer) {
    if (buffer.length & 0x7FFF) {
      throw new Error('TODO')
    } else {
      return this.calculateChecksumMirror(buffer, 0, buffer.length, 0x800000);
    }
  }
}
