export default class MemoryAreas {
  constructor(size) {
    this.size = size;
    this.areas = [[0, size]];
  }

  clone() {
    const copy = new MemoryAreas(this.size);
    copy.areas = [];
    for (let area of this.areas) {
      copy.areas.push(area.slice());
    }
    return copy;
  }

  inverted() {
    const invertedInstance = new MemoryAreas(this.size);
    invertedInstance.areas = [];
    var offset = 0;

    for (let area of this.areas) {
      var newOffset = area[0];
      if (newOffset > offset) {
        invertedInstance.areas.push([offset, newOffset - offset]);
      }
      offset = newOffset + area[1];
    }

    if (offset < this.size) {
      invertedInstance.areas.push([offset, this.size - offset + 1]);
    }

    return invertedInstance;
  }

  union(other) {
    var leftAreaIndex = 0;
    var rightAreaIndex = 0;

    const unionInstance = new MemoryAreas(this.size);
    unionInstance.areas = [];

    while (leftAreaIndex < this.areas.length && rightAreaIndex < other.areas.length) {
      const left = this.areas[leftAreaIndex];
      const right = other.areas[rightAreaIndex];
      const leftEnd = left[0] + left[1];
      const rightEnd = right[0] + right[1];

      const newStart = Math.max(left[0], right[0]);
      const newEnd   = Math.min(leftEnd, rightEnd);
      const newSize  = newEnd - newStart;

      if (newSize > 0) {
        unionInstance.areas.push([newStart, newSize]);
      }

      if (leftEnd >= rightEnd) {
        rightAreaIndex++;
      }
      if (rightEnd >= leftEnd) {
        leftAreaIndex++;
      }
    }

    return unionInstance;
  }

  getBlockAt(index) {
    return this.areas[index];
  }

  getIndexOfAddress(address) {
    var currentIndex, currentElement;
    var minIndex = 0;
    var maxIndex = this.areas.length - 1;

    while (minIndex <= maxIndex) {
      currentIndex = (minIndex + maxIndex) >> 1;
      currentElement = this.areas[currentIndex];

      if ((currentElement[0] + currentElement[1]) < address) {
        minIndex = currentIndex + 1;
      } else if (currentElement[0] > address) {
        maxIndex = currentIndex - 1;
      } else {
        return currentIndex;
      }
    }

    return null;
  }

  block(address, size) {
    if (!size) {
      return;
    }

    const index = this.getIndexOfAddress(address);
    if (index === null) {
      throw new Error(`The requested address $${address.toString(16)} is already used by another block`);
    }

    const block = this.areas[index];
    const sizeBeforeData = address - block[0];
    const sizeAfterData = block[1] - sizeBeforeData - size;
    if (sizeBeforeData < 0 || sizeAfterData < 0) {
      throw new Error(`The requested address $${address.toString(16)} is not able to reserve ${size} bytes, because it is already used by another block`);
    }

    var newBlocks = [];
    if (sizeBeforeData) {
      newBlocks.push([block[0], sizeBeforeData]);
    }
    if (sizeAfterData) {
      newBlocks.push([address + size, sizeAfterData]);
    }

    this.areas.splice(index, 1, ...newBlocks);
  }

  unblockArea(area) {
    const clone = this.clone();

    for (let block of area.areas) {
      clone.unblock(block[0], block[1]);
    }

    return clone;
  }

  unblockAt(index, address, size) {
    this.areas.splice(index, 0, [address, size]);

    if (index > 0) {
      const left = this.areas[index - 1];

      if (left[0] + left[1] >= address) {
        var size = address + size - left[0];
        if (size < left[1]) {
          size = left[1];
        }
        this.areas.splice(index - 1, 2, [left[0], size])
      }

      index--;
    }

    if (index + 1 < this.areas.length) {
      const left = this.areas[index];
      const right = this.areas[index + 1];

      if (left[0] + left[1] >= right[0]) {
        this.areas.splice(index, 2, [left[0], right[0] + right[1] - left[0]])
      }
    }
  }

  unblock(address, size) {
    const end = address + size - 1;

    for (var i=0; i<this.areas.length; i++) {
      const area = this.areas[i];
      const left = area[0];
      const right = left + area[1] - 1;

      if (right < address) {
        continue;
      }
      if (left > end) {
        this.unblockAt(i, address, size);
        return;
      }
    }

    this.unblockAt(this.areas.length, address, size);
  }

  differenceTo(address, size) {
    const input = new MemoryAreas(size);
    input.areas[0][0] = address;

    return this.union(input).areas;
  }

  dump() {
    var offset = 0;

    const dumpOffset = (a) => {
      a = a.toString(16)
      a = '0000'.substr(a.length) + a;
      return '$' + a;
    }

    for (let area of this.areas) {
      var newOffset = area[0];

      if (newOffset !== offset) {
        console.log(`${dumpOffset(offset)}-${dumpOffset(newOffset-1)} : RESERVED`);
      }

      console.log(`${dumpOffset(newOffset)}-${dumpOffset(newOffset + area[1] - 1)} : -`);
      offset = newOffset + area[1];
    }

    if (offset < this.size) {
      console.log(`${dumpOffset(offset)}-${dumpOffset(this.size - 1)} : RESERVED`);
    }

    console.log('----');
  }
}
