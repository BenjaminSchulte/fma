import {Assembler as CoreAssembler} from '../core';
import BufferSizeModifier from './BufferSizeModifier'
import ChecksumCalculator from './ChecksumCalculator'
import LinkerResult from '../../linker/LinkerResult'

export default class Assembler extends CoreAssembler {
  getName() {
    return 'SNES 65816';
  }

  postProcess(result) {
    const applicationAddress = result.getSymbols().get('.rom_header_location');
    const romAddress = result.getRomCalculator().getRomAddress(applicationAddress);
    const headerOffset = result.getRomCalculator().getRomOffset(romAddress);

    const sizeFixer = new BufferSizeModifier();
    const buffer = sizeFixer.resize(result.getBinary(), headerOffset);

    const calculator = new ChecksumCalculator();
    calculator.calculate(buffer, headerOffset);

    return new LinkerResult(
      buffer,
      result.getSymbols(),
      result.getRomCalculator()
    )
  }
}
