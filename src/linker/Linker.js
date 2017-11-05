import LinkerObject from './LinkerObject';
import SymbolList from './SymbolList';

export default class Linker {
  constructor(project) {
    this.project = project;
    this.object = new LinkerObject();
  }

  addObject(object) {
    this.object.merge(object);
  }

  log(...args) {
    this.project.log(...args);
  }

  addStaticCodeToRom() {
    for (let block of this.object.getStaticCodeBlocks()) {
      block.registerInMemory(this.object.getRomBlock())
    }
  }

  collectSymbols() {
    const symbols = new SymbolList();
    for (let block of this.object.getStaticCodeBlocks()) {
      block.collectSymbols(symbols);
    }
    this.object.getRomBlock().collectSymbols(symbols);
    this.object.getRamBlock().collectSymbols(symbols);
    return symbols;
  }

  fillCalculations(symbols) {
    for (let block of this.object.getStaticCodeBlocks()) {
      block.fillCalculations(symbols);
    }
  }

  link() {
    this.addStaticCodeToRom();
    this.object.getRomBlock().build();
    this.object.getRamBlock().build();

    const symbols = this.collectSymbols();

        this.object.getRomBlock().memory.dump();
        this.object.getRamBlock().memory.dump();

    this.fillCalculations(symbols);

  }
}
