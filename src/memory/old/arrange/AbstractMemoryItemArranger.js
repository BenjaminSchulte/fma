import MemoryLocation from '../MemoryLocation';
import MemoryConditionJoiner from './MemoryConditionJoiner';

export default class AbstractMemoryItemArranger {
  shouldRespectLocation(location) {
    return false;
  }

  getLocations(node) {
    return node.getAllowed().filter(location => this.shouldRespectLocation(location));
  }

  arrange(node) {
    const size = node.getTotalSize();
    if (!size) {
      return;
    }

    const locations = this.getLocations(node);
    const target = node.sections.getBestLocation(locations, size);
    if (!target) {
      throw new Error(`Can not allocate address for node. Size: ${size}`);
    }

    this.arrangeAt(node, target.section, target.address, size);
  }

  arrangeAt(node, section, address, size) {
    const joiner = new MemoryConditionJoiner();
    const range = MemoryLocation.inRange(address, size).inBank(section.node.address);

    node.setStaticAddress(section.allocate(address, size));

    joiner.joinChildren(node, [range]);
  }
}
