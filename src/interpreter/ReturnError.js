export default class ReturnError extends Error {
  constructor(value) {
    super();

    this.message = "Return statements are not valid at this point of the source code";
    this.isReturn = true;
    this.doNotShowError = true;
    this.returnValue = value;
  }
}
