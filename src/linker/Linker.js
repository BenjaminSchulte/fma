import LinkerObject from './LinkerObject';
import SymbolList from './SymbolList';
import RomWriter from './writer/RomWriter';
import LinkerResult from './LinkerResult';
import LinkerCommands from './LinkerCommands';

export default class Linker {
  constructor(project) {
    this.project = project;
    this.object = new LinkerObject();
    this.commands = new LinkerCommands();
  }

  addObject(object) {
    this.object.merge(object);
  }

  addCommand(command) {
    return this.commands.add(command);
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
      block.fillCalculations(symbols, this);
    }

    for (let configuration of this.object.getConfigurations()) {
      this.commands.addConfiguration(configuration.calculate(symbols, this));
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

    var result = new LinkerResult(
      rom.getBuffer(),
      symbols,
      this.object.getRomBlock().getMemoryRecalculator(),
      this.commands
    );

    for (let plugin of this.project.getPlugins().reverse()) {
      result = plugin.postProcess(this.project, result);
    }

    return result;
  }
}
