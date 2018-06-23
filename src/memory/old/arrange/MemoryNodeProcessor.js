import MemoryLocation from '../MemoryLocation';
import MemoryConditionJoiner from '../_arrange/MemoryConditionJoiner';

export default class MemoryNodeProcessor {
  hasStaticAddress(node) {
    return this.getLocations(node).some(location => location.isStaticAddress());
  }

  needsToBeProcessed(node) {
    if (node.isRootNode()) {
      return false;
    }

    if (this.hasStaticAddress(node)) {
      return true;
    }

    const size = node.getOwnSectionSize();
    if (!size) {
      return false;
    }

    return true;
  }

  isProcessed(node) {
    return node.hasStaticAddress();
  }

  allParentsAreProcessed(node) {
    for (let parent of node.getParents()) {
      if (this.needsToBeProcessed(parent) && !this.isProcessed(parent)) {
        if (this.canBeProcessed(parent)) {
          return this.process(parent);
        }
        return false;
      }

      if (!this.allParentsAreProcessed(parent)) {
        return false;
      }
    }

    return true;
  }

  isStringedContentWithUnprocessedSiblings(node) {
    if (!node.isStringedTogether()) {
      return false;
    }

    const parents = node.getParents().filter(parent => parent.isStringedTogether());
    for (let parent of parents) {
      for (let child of parent.getChildren()) {
        if (child === node) {
          break;
        }

        if (this.needsToBeProcessed(child) && !this.isProcessed(child)) {
          if (!this.canBeProcessed(child) || !this.process(child)) {
            return true;
          }
        }
      }
    }

    return false;
  }

  canBeProcessed(node) {
    if (!this.allParentsAreProcessed(node)) {
      return false;
    }

    if (this.isStringedContentWithUnprocessedSiblings(node)) {
      return false;
    }

    return true;
  }

  getLocations(node) {
    return node.getAllowed();
  }

  process(node) {
    if (this.isProcessed(node)) {
      return true;
    }

    if (!this.needsToBeProcessed(node)) {
      //console.log('SKIP:');
      //node.dump('', 0);
      //this.__dumpParents(node);

      return true;
    }

    if (!this.canBeProcessed(node)) {
      /*console.log('POSTPONE:');
      node.dump('', 0);
      this.__dumpParents(node);
      console.log('---');*/

      return false;
    }

    //console.log('PROCESS: ' + node.ratePriority());
    //node.dump();

    const size = node.getTotalSize();
    const locations = this.getLocations(node);
    const target = node.sections.getBestLocation(locations, size);
    if (!target) {
      node.sections.dumpUsage();
      node.dump('', 0);
      this.__dumpParents(node);
      throw new Error(`Can not allocate address for node. Size: ${size}`);
    }

    node.setStaticAddress(target.section.allocate(target.address, size));

    const joiner = new MemoryConditionJoiner();
    const range = MemoryLocation.inRange(target.address, size).inBank(target.section.node.address)
    joiner.joinChildren(node, [range]);

    //node.dump();

    return true;
  }

  __dumpParents(node, spaces=". ") {
    for (let parent of node.getParents()) {
      if (parent.constructor.name === 'Memory') {
        continue;
      }
      process.stdout.write((this.needsToBeProcessed(parent) ? "Y" : "N") + ' : ');
      parent.dump(spaces, 0);

      this.__dumpParents(parent, spaces+". ")
    }
  }
}
