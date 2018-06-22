import AbstractMemoryItemArranger from './AbstractMemoryItemArranger';

export default class MemoryFixedAddressArranger extends AbstractMemoryItemArranger {
  shouldRespectLocation(location) {
    return location.isStaticAddress();
  }
}
