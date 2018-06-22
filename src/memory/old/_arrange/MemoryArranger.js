import MemoryConditionJoiner from './MemoryConditionJoiner';
import AnyMemoryItemArranger from './AnyMemoryItemArranger';
import MemoryFixedAddressArranger from './MemoryFixedAddressArranger';
import MemoryFixedBankArranger from './MemoryFixedBankArranger';
import MemoryAlignedAddressArranger from './MemoryAlignedAddressArranger';

export default class MemoryArranger {
  constructor(memory) {
    this.memory = memory;
  }

  joinConditions() {
    const joiner = new MemoryConditionJoiner();
    joiner.joinChildren(this.memory);
  }

  processFixedAddresses(node, onlyLinked=false) {
    const arranger = new MemoryFixedAddressArranger();
    this.walk(node, onlyLinked, location => location.isStaticAddress(), node => arranger.arrange(node));
  }

  processFixedBanks(node, onlyLinked=false) {
    const arranger = new MemoryFixedBankArranger();
    this.walk(node, onlyLinked, location => location.isStaticBank(), node => {
      if (node.getOwnSectionSize()) {
        arranger.arrange(node)
      }
    });
  }

  processAlignedAddresses(node, onlyLinked=false) {
    const arranger = new MemoryAlignedAddressArranger();
    this.walk(node, onlyLinked, location => location.isAlignedAddress(), node => arranger.arrange(node));
  }

  processOther(node, onlyLinked=false) {
    const arranger = new AnyMemoryItemArranger();
    this.walk(node, onlyLinked, location => true, node => {
      if (node.getOwnSectionSize()) {
        arranger.arrange(node)
      }
    });
  }

  walk(node, onlyLinked, filter, callback) {
    const locations = node.getAllowed().filter(filter);
    if (locations.length && !node.hasStaticAddress()) {
      if (!onlyLinked || node.isLinked()) {
        try {
          callback(node);
        } catch(err) {
          this.memory.dump();
          console.log('---');
          node.dump();
          throw new Error('Error processing node ' + (node._name || '') + ': ' + err.toString());
        }
        onlyLinked = false;
      }
    }

    node.getChildren().forEach(child => this.walk(child, onlyLinked, filter, callback));
  }

  arrange() {
    //this.memory.getChildren().forEach(child => this.dumpAll(child));

    this.joinConditions();

console.log('PROCESS FIXED')
    this.memory.getChildren().forEach(child => this.processFixedAddresses(child));
console.log('PROCESS ALIGNED (LINKED)')
    this.memory.getChildren().forEach(child => this.processAlignedAddresses(child, true));
console.log('PROCESS WITH BANK (LINKED)')
    this.memory.getChildren().forEach(child => this.processFixedBanks(child, true));
console.log('PROCESS OTHER (LINKED)')
    this.memory.getChildren().forEach(child => this.processOther(child, true));
console.log('PROCESS ALIGNED (ALL)')
    this.memory.getChildren().forEach(child => this.processAlignedAddresses(child));
console.log('PROCESS WITH BANK (ALL)')
    this.memory.getChildren().forEach(child => this.processFixedBanks(child));
console.log('PROCESS OTHER (ALL)')
    this.memory.getChildren().forEach(child => this.processOther(child));
  }

  dumpAll(child) {
    child.dump();

    child.getChildren().forEach(child => this.dumpAll(child));
  }
}
