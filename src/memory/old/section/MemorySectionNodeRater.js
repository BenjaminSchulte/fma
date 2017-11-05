import MemoryAreas from './MemoryAreas';

export default class MemorySectionNodeRater {
  constructor(node) {
    this.node = node;
  }

  rate(location, size) {
    const areas = this.getAreaInRange(location, this.node.getFreeAreas());

    if (location.addresses.length) {
      return this.rateByAddresses(areas, location.addresses, size);
    }

    if (location.alignedToAddresses.length) {
      return this.rateByAlignedAddresses(areas, location.alignedToAddresses, size);
    }

    return this.rateByAnyAddress(areas, size);
  }

  getAreaInRange(location, areas) {
    if (!location.addressRanges.length) {
      return areas;
    }

    const ranges = new MemoryAreas(areas.size);
    ranges.block(0, areas.size);

    for (let range of location.addressRanges) {
      ranges.unblock(range[0], range[1]);
    }

    return ranges.union(areas);
  }

  rateByAnyAddress(areas, size) {
    var possibilities = [];

    for (let area of areas.areas) {
      if (area[1] < size) {
        continue;
      }

      possibilities = possibilities.concat(this.rateAddress(areas, area[0], size));
    }

    return possibilities;
  }

  rateByAlignedAddresses(areas, addresses, size) {
    return addresses.map(address => this.rateAlignedAddress(areas, address, size)).reduce((a, b) => a.concat(b), []);
  }

  rateAlignedAddress(areas, address, size) {
    var possibilities = [];

    for (let area of areas.areas) {
      const start = area[0];
      const end = start + area[1];

      var offset = Math.floor((start + address - 1) / address) * address;
      if (offset + size < end) {
        possibilities = possibilities.concat(this.rateAddress(areas, offset, size));
      }
    }

    return possibilities;
  }

  rateByAddresses(areas, addresses, size) {
    return addresses.map(address => this.rateAddress(areas, address, size)).reduce((a, b) => a.concat(b), []);
  }

  rateAddress(areas, address, size) {
    const index = areas.getIndexOfAddress(address);
    if (index === null) {
      return [];
    }

    const block = areas.getBlockAt(index);
    const memBefore = address - block[0];
    const memAfter  = block[1] - memBefore - size;

    if (memBefore < 0 || memAfter < 0) {
      return [];
    }

    const score = memBefore * 10 + memAfter;

    return [{
      section: this.node,
      address: address,
      score: score
    }];
  }
}
