import winston from 'winston';
import CorePlugin from '../types/CorePlugin';
import DirectorySource from '../grammar/source/DirectorySource';

export default class Project {
  constructor() {
    this.currentLog = null;

    this.plugins = []
    //this.includeDirs = [];
    this.fileSources = [];
    this.registerPlugin(new CorePlugin());
  }

  addFileSource(source) {
    this.fileSources.push(source);
  }

  addIncludeDir(dir) {
    this.addFileSource(new DirectorySource(dir));
  }

  /*getIncludeDirs() {
    return this.includeDirs.slice();
  }*/

  registerPlugin(plugin) {
    this.plugins.push(plugin);
  }

  getPlugins() {
    return this.plugins.slice();
  }

  setLog(log) {
    this.currentLog = log;
  }

  log(...args) {
    if (!this.currentLog) {
      this.createDefaultLogger();
    }

    this.currentLog.log(...args);
  }

  createDefaultLogger() {
    this.setLog(winston.createLogger({
      level: 'info',
      transports: [new winston.transports.Console()],
      format: winston.format.combine(
        winston.format.colorize({ all: true }),
        winston.format.simple()
      )
    }))
  }
}
