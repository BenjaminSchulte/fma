import ClassInstance from '../objects/ClassInstance';
import CompilerTypeClass from './CompilerTypeClass';

export default class CompilerType extends ClassInstance {
  constructor() {
    super(new CompilerTypeClass());
  }
}
