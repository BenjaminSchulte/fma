export default class MemoryAllocationContainer {
  constructor(sections) {
    this.children = [];
    this.sections = sections;
  }

  isRootNode() {
    return false;
  }

  createChildSections() {
    return this.sections;
  }

  allocate() {
    const MemoryAllocation = require('./MemoryAllocation').default;
    const allocation = new MemoryAllocation(this.createChildSections(), this);
    this.children.push(allocation);
    return allocation;
  }

  detachChild(child) {
    const index = this.children.indexOf(child);

    if (index >= 0) {
      this.children.splice(index, 1);
    }
  }

  getChildren() {
    return this.children;
  }
}
