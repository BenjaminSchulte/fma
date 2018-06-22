import AbstractMemoryItemArranger from './AbstractMemoryItemArranger';

export default class MemoryFixedBankArranger extends AbstractMemoryItemArranger {
  shouldRespectLocation(location) {
    return location.isStaticBank();
  }
}
