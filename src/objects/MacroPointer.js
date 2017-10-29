import AbstractObject from './Object';

export default class MacroPointer extends AbstractObject {
  constructor(macro) {
    super();

    this.macro = macro;
  }

  type() {
    return 'MacroPointer';
  }

  getMacro() {
    return this.macro;
  }
}
