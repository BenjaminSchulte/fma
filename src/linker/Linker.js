import LinkerObject from './LinkerObject';
import SymbolList from './SymbolList';
import RomWriter from './writer/RomWriter';
import LinkerResult from './LinkerResult';

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

  build() {
    const builder = new RomWriter();
    for (let block of this.object.getStaticCodeBlocks()) {
      builder.add(block);
    }
    return builder.get();
  }

  link() {
    this.addStaticCodeToRom();

    this.log('info', 'Arranging memory blocks.');
    this.object.getRomBlock().build();
    this.object.getRamBlock().build();

    this.log('info', 'Resolving missing symbols.');
    const symbols = this.collectSymbols();
    this.fillCalculations(symbols);

    this.log('info', 'Building ROM file.');
    const rom = this.build();

    return new LinkerResult(rom.getBuffer(), symbols, this.object.getRomBlock().getMemoryRecalculator());
  }
}
