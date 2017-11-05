import MemoryConditionJoiner from './MemoryConditionJoiner';
import AnyMemoryItemArranger from './AnyMemoryItemArranger';
import MemoryFixedAddressArranger from './MemoryFixedAddressArranger';
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

    this.memory.getChildren().forEach(child => this.processFixedAddresses(child));
    this.memory.getChildren().forEach(child => this.processAlignedAddresses(child, true));
    this.memory.getChildren().forEach(child => this.processOther(child, true));
    this.memory.getChildren().forEach(child => this.processAlignedAddresses(child));
    this.memory.getChildren().forEach(child => this.processOther(child));
  }

  dumpAll(child) {
    child.dump();

    child.getChildren().forEach(child => this.dumpAll(child));
  }
}
