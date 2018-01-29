import MemorySectionNode from './MemorySectionNode';
import MemorySectionNodeRater from './MemorySectionNodeRater';
import Bank from './Bank';

export default class MemorySections {
  constructor() {
    this.id = "1";
    this.__nextChildId = 1;

    this.sections = [];
  }

  attach(sections) {
    sections.id = `${sections.id}@(${this.id})`;

    for (let section of sections.sections) {
      if (!(section.node instanceof Bank)) {
        throw new Error('Can not attach sections that are no bank');
      }

      section.attachParent(this.getSectionAtBank(section.node.address));
    }
  }

  detach() {
    for (let section of this.sections) {
      section.detach();
    }
  }

  register(section) {
    this.sections.push(new MemorySectionNode(section));
  }

  allocate(locations, size) {
    const target = this.getBestLocation(locations, size);
    if (!target) {
      throw new Error('Could not arrange memory block');
    }

    return target.section.allocate(target.address, size);
  }

  getBestLocation(locations, size) {
    const rated = this.rate(locations, size);
    if (!rated.length) {
      return null;
    }

    return rated[0];
  }

  rate(locations, size) {
    var ratings = [];

    for (let location of locations) {
      ratings = ratings.concat(this.rateLocation(location, size));
    }

    ratings.sort((a, b) => a.score - b.score);
    return ratings;
  }

  getSectionAtBank(address) {
    return this.sections.find(section => section.node.isBank([address]));
  }

  rateLocation(location, size) {
    var sections = this.sections;

    if (location.sections.length) {
      sections = location.sections;
    } else if (location.banks.length) {
      sections = sections.filter(section => section.node.isBank(location.banks));
    }

    var ratings = [];
    for (let section of sections) {
      const rater = new MemorySectionNodeRater(section);
      ratings = ratings.concat(rater.rate(location, size));
    }

    return ratings;
  }

  createChild() {
    const child = new MemorySections();

    ////// TEMP!!!!!!
    child.id = this.id + ":" + this.__nextChildId++;
    child.__nextChildId = 1;

    for (let section of this.sections) {
      child.sections.push(new MemorySectionNode(section.node, section));
    }
    return child;
  }

  setIsShared(shared) {
    for (let section of this.sections) {
      section.setIsShared(true);
    }
  }

  dumpUsage(banks) {
    for (let section of this.sections.filter(section => section.node.isBank(banks))) {
      section.dumpUsage();
    }
  }
}
