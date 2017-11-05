import MemoryAllocationContainer from './MemoryAllocationContainer';
import MemorySections from './section/MemorySections';
import MemorySectionNode from './section/MemorySectionNode';
import Bank from './section/Bank';

export default class Memory extends MemoryAllocationContainer {
  constructor() {
    super(new MemorySections());
  }

  isRootNode() {
    return true;
  }

  createSection(size, bankNumber=0) {
    return new MemorySectionNode(new Bank(bankNumber, size));
  }

  registerSection(section) {
    this.sections.register(section);
    return section;
  }

  registerBank(address, size=0x10000) {
    const bank = new Bank(address, size);
    this.sections.register(bank);
    return bank;
  }

  dump() {
    for (let child of this.children) {
      child.dump();
    }
  }
}
