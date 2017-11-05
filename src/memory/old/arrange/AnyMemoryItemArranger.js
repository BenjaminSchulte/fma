import AbstractMemoryItemArranger from './AbstractMemoryItemArranger';

export default class AnyMemoryItemArranger extends AbstractMemoryItemArranger {
  shouldRespectLocation(location) {
    return true;
  }
}
