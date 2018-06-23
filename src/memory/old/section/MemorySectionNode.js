import MemoryAreas from './MemoryAreas';
import StaticAddress from '../StaticAddress';

var nextId = 1;

export default class MemorySectionNode {
  /**
   * Constructor
   */
  constructor(node, parent=null) {
    this.id = nextId++;
    this.node = node;
    this.parents = parent === null ? [] : [parent];
    this.shared = false;
    this.children = [];

    this.freeAreas = null;
    this.reservedAreas = new MemoryAreas(node.size);
    this.localFreeAreas = new MemoryAreas(node.size);

    if (parent) {
      parent.children.push(this);
    }
  }

  /**
   * Attaches a new parent
   */
  attachParent(parent) {
    this.parents.push(parent);
    parent.children.push(this);

    this.invalidate();
  }

  /**
   * Detaches a parent
   */
  detachParent(parent) {
    const index = this.parents.indexOf(parent);

    if (index >= 0) {
      this.parents.splice(index, 1);

      const parentIndex = parent.children.indexOf(this);
      if (parentIndex >= 0) {
        parent.children.splice(parentIndex, 1);
      }
    }

    this.invalidate();
  }

  /**
   * Detaches all parents
   */
  detach() {
    while (this.parents.length) {
      this.detachParent(this.parents[0]);
    }
  }

  /**
   * Sets whether this section is shared or not
   */
  setIsShared(shared) {
    this.shared = true;
  }

  /**
   * Invalidates this node and all its children
   */
  invalidate() {
    if (!this.freeAreas) {
      return;
    }

    this.freeAreas = null;

    for (let child of this.children) {
      child.invalidate()
    }
  }

  /**
   * Returns all free areas for children
   */
  getFreeAreas() {
    if (this.freeAreas) {
      return this.freeAreas;
    }

    var parentAreas = this.getParentFreeAreas();
    this.freeAreas = parentAreas.unblockArea(this.localFreeAreas.inverted()).union(this.reservedAreas);

    return this.freeAreas;
  }

  /**
   * Returns the free areas of all parents
   */
  getParentFreeAreas() {
    var area = new MemoryAreas(this.node.size);

    for (let parent of this.parents) {
      area = parent.getFreeAreas().union(area);
    }

    return area;
  }

  /**
   * Blocks memory
   */
  block(address, size) {
    if (!this.shared) {
      this.reservedAreas.block(address, size);
    }

    var requestedBlockArea = new MemoryAreas(this.localFreeAreas.size);

    requestedBlockArea.block(address, size);
    requestedBlockArea = requestedBlockArea.inverted().union(this.localFreeAreas);
    this.invalidate();

    for (let block of requestedBlockArea.areas) {
      const blockAddress = block[0];
      const blockSize = block[1];

      this.localFreeAreas.block(blockAddress, blockSize);
      this.invalidate();

      for (let parent of this.parents) {
        parent.block(blockAddress, blockSize);
      }
    }
  }

  /**
   * Allocates memory
   */
  allocate(address, size) {
    for (let parent of this.parents) {
      parent.block(address, size);
    }
    this.localFreeAreas.block(address, size);
    this.invalidate();

    return new StaticAddress(this.node, address, size);
  }

  dumpUsage() {
    const bank = this.node.address ? this.node.address.toString(16).padStart(2, '0') : '00';

    console.log("BANK: " + bank);
    this.getFreeAreas().dump()
  }

  dumpTree() {
    var root = this;
    while (root.parents.length) root = root.parents[0];

    const dump = (node, prefix) => {
      console.log(`${prefix}-- #${node.id} RESERVE: ` + node.reservedAreas.areas.join(' + '));
      console.log(`${prefix}-- #${node.id} LOCAL  : ` + node.localFreeAreas.areas.join(' + '));
      console.log(`${prefix}-- #${node.id} FREE   : ` + node.getFreeAreas().areas.join(' + '));

      for (let child of node.children) {
        dump(child, '  ' + prefix);
      }
    }
    dump(root, '')
  }
}
