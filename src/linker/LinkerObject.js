export default class LinkerObject {
  constructor() {
    this.staticCodeBlocks = [];
    this.configurations = [];
    this.romBlock = null;
    this.ramBlock = null;
  }

  addConfiguration(configuration) {
    this.configurations.push(configuration);
  }

  getConfigurations() {
    return this.configurations;
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
    } else if (block === null) {
    } else {
      this.romBlock = this.romBlock.merge(block);
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
    } else if (block === null) {
    } else {
      this.ramBlock = this.ramBlock.merge(block);
    }
  }

  getRamBlock() {
    if (!this.ramBlock) {
      return null;
    }

    return this.ramBlock;
  }

  merge(other) {
    this.staticCodeBlocks = this.staticCodeBlocks.concat(other.staticCodeBlocks);
    this.configurations = this.configurations.concat(other.configurations);
    this.addRomBlock(other.romBlock);
    this.addRamBlock(other.ramBlock);
  }

  serialize() {
    return {
      staticCodeBlocks: this.staticCodeBlocks.map(block => block.serialize())
    }
  }
}
