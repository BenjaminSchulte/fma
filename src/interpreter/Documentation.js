export default class Documentation {
  constructor(object) {
    this.object = object;
    this.text = '';
    this.invalid = true;
  }

  update() {
    if (!this.invalid) {
      return;
    }

    this.properties = {};

    var text = this.text.replace(/^@([a-z]+)(\s+(.*))?\s*$/gm, (all, name, _2, content) => {
      if (!this.properties.hasOwnProperty(name)) {
        this.properties[name] = [];
      }

      this.properties[name].push(content);
      return '';
    })

    var lines = text.split(/\r?\n/);
    var summary = [];
    var content = [];
    var hasSummary = false;
    var isSummary = true;
    for (let line of lines) {
      if (!isSummary) {
        content.push(line);
      } else if (line.trim() === '') {
        if (hasSummary) {
          isSummary = false;
        }
      } else {
        hasSummary = true;
        summary.push(line);
      }
    }

    summary = summary.join("\n")
    content = content.join("\n");

    if (!this.properties.hasOwnProperty('summary')) {
      this.properties.summary = [summary.trim()];
    }

    this.content = content.trim() || null;
    this.invalid = false;
  }

  getProperties(name) {
    this.update();

    return this.properties[name] || [];
  }

  getProperty(name) {
    const value = this.getProperties(name);

    if (!value.length) {
      return null;
    }

    return value[0];
  }

  getParameters() {
    return this.getProperties('param').map(param => {
      var match = param.match(/^([a-zA-Z0-9_]+\??)(=([^\s]+))?\s+(.*)\s*$/);
      if (!match) {
        return {name: param}
      } else {
        return {
          name: match[1],
          defaultValue: match[3],
          text: match[4]
        }
      }
    })
  }

  getSummary() {
    return this.getProperty('summary');
  }

  getPackage() {
    return this.getProperty('package');
  }

  getDescription() {
    this.update();
    return this.content;
  }

  hasAny() {
    return !!this.text.length;
  }

  add(text) {
    if (text === null || text === '') {
      return;
    }

    this.invalid = true;
    text = text.trim().replace(/\n[ ]/g, "\n");

    this.text += "\n" + text;
  }
}
