import winston from 'winston';
import CorePlugin from '../types/CorePlugin';

export default class Project {
  constructor() {
    this.currentLog = null;

    this.plugins = []
    this.includeDirs = [];
    this.registerPlugin(new CorePlugin());
  }

  addIncludeDir(dir) {
    this.includeDirs.push(dir);
  }

  getIncludeDirs() {
    return this.includeDirs.slice();
  }

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
