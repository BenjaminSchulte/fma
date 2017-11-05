import LinkerObject from './LinkerObject';

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

  link() {
    this.addStaticCodeToRom();
    this.object.getRomBlock().build();
    this.object.getRamBlock().build();

    console.log('');
    console.log('<<ROM>>');
    this.object.getRomBlock().memory.dump();
    console.log('');
    console.log('<<RAM>>');
    this.object.getRamBlock().memory.dump();
  }
}
