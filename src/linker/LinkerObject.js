export default class LinkerObject {
  constructor() {
    this.staticCodeBlocks = [];
    this.romBlock = null;
    this.ramBlock = null;
  }

  addStaticCode(block) {
    this.staticCodeBlocks.push(block);
  }

  getStaticCodeBlocks() {
    return this.staticCodeBlocks;
  }

  addRomBlock(block) {
    if (this.romBlock === null) {
      this.romBlock = block;
    } else {
      this.romBlock.merge(block);
    }
  }

  getRomBlock() {
    if (!this.romBlock) {
      throw new Error('No ROM block has been defined');
    }

    return this.romBlock;
  }

  addRamBlock(block) {
    if (this.ramBlock === null) {
      this.ramBlock = block;
    } else {
      this.ramBlock.merge(block);
    }
  }

  getRamBlock() {
    if (!this.ramBlock) {
      throw new Error('No RAM block has been defined');
    }

    return this.ramBlock;
  }

  merge(other) {
    this.staticCodeBlocks = this.staticCodeBlocks.concat(other.staticCodeBlocks);
    this.addRomBlock(other.romBlock);
    this.addRamBlock(other.ramBlock);
  }
}
