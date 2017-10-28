import winston from 'winston';

export default class Project {
  constructor() {
    this.currentLog = null;
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
