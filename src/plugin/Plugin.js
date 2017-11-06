export default class Plugin {
  getName() {
    return 'Untitled plugin';
  }

  preProcess(project, interpreter) {
  }

  postProcess(project, result) {
    return result;
  }
}
