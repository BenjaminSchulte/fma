export default class VariableTypes {
  static getSizeOf(type) {
    switch (type) {
      case 'uint8': return 1;
      case 'uint16': return 2;
      case 'uint24': return 3;
      case 'uint32': return 4;
      default:
        throw new Error(`Unknown item type: ${type}`);
    }
  }
}
