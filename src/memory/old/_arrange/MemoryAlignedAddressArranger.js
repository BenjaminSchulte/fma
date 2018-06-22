import AbstractMemoryItemArranger from './AbstractMemoryItemArranger';

export default class MemoryAlignedAddressArranger extends AbstractMemoryItemArranger {
  shouldRespectLocation(location) {
    return location.isAlignedAddress();
  }
}
