import MemoryLocation from '../MemoryLocation';

class Item {
  constructor(node, parent) {
    this.node = node;
    this.parent = parent;
    this.score = null;
  }

  getLocationScore(location) {
    const flags = location.flags();
    var score = 0;

    if (flags & MemoryLocation.FLAG_DEPENDENCIES) {
      score += 13;
    }
    if (flags & MemoryLocation.FLAG_STATIC_ADDRESS) {
      score += 11;
    }
    if (flags & MemoryLocation.FLAG_ALIGNED_ADDRESS) {
      score += 7;
    }
    if (flags & MemoryLocation.FLAG_STATIC_BANK) {
      score += 5;
    }
    if (flags & MemoryLocation.FLAG_STATIC_SECTION) {
      score += 3;
    }

    return score;
  }

  getSize() {
    return this.node.getOwnSectionSize();
  }

  getScore() {
    if (this.score !== null) {
      return this.score;
    }

    var scoreSum = null;
    var scoreCount = 0;
    for (let allow of this.node.getAllowed()) {
      scoreSum += this.getLocationScore(allow);
      scoreCount++;
    }

    var score = scoreCount ? scoreSum / scoreCount : 0;
    if (this.parent) {
      score += this.parent.getScore() * 50;
    }
    this.score = score;
    return score;
  }
}

export default class MemoryAllocationDatabase {
  constructor(parent) {
    this.nextItemId = 1;

    this.items = [];
  }

  add(node, parent) {
    const item = new Item(node, parent);
    this.items.push(item);
    return item;
  }

  sort() {
    this.items.sort((b, a) => {
      const score = a.getScore() - b.getScore();
      if (score === 0) {
        return a.getSize() - b.getSize();
      }
      return score;
    })
  }

  getItems() {
    return this.items.map(item => item.node);
  }
}
