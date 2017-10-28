import {Assembler as CoreAssembler} from '../core';

export default class Assembler extends CoreAssembler {
  getName() {
    return 'SNES 65816';
  }
}
