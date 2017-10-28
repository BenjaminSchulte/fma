export default class InterpreterError extends Error {
  constructor(message) {
    super();

    this.message = message;
    this.isInterpreterError = true; //workaround for some babel problems
  }
}
