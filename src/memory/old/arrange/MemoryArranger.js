import MemoryNodeProcessor from './MemoryNodeProcessor';
import MemoryConditionJoiner from '../_arrange/MemoryConditionJoiner';

export default class MemoryArranger {
  constructor(memory) {
    this.memory = memory;
  }

  joinConditions() {
    const joiner = new MemoryConditionJoiner();
    joiner.joinChildren(this.memory);
  }

  _flattenNodes(result, node) {
    for (let child of node.getChildren()) {
      result.push(child);

      this._flattenNodes(result, child);
    }
  }

  flattenNodes(node) {
    var fullResult = [];
    this._flattenNodes(fullResult, this.memory);

    var result = [];
    for (let item of fullResult) {
      if (result.indexOf(item) === -1) {
        result.push(item);
      }
    }
    return result;
  }

  priorizeNodes(nodes) {
    return nodes.sort((a, b) => {
      const r = b.ratePriority() - a.ratePriority();
      if (r !== 0) {
        return r;
      }

      return a.siblingIndex() - b.siblingIndex();
    })
  }

  arrange() {
    this.joinConditions();

    const processor = new MemoryNodeProcessor();
    var nodes = this.priorizeNodes(this.flattenNodes());
    while (nodes.length) {
      var numNodes = nodes.length;
      var unprocessedNodes = [];

      for (let i=0; i<numNodes; i++) {
        const node = nodes[i];

        if (!processor.process(node)) {
          unprocessedNodes.push(node);
        } else if (unprocessedNodes.length) {
          nodes = unprocessedNodes.concat(nodes.slice(i + 1));
          unprocessedNodes = [];
          numNodes = nodes.length;
          i = 0;
          console.log(nodes.length);
        }
      }

      if (unprocessedNodes.length === numNodes) {
        this.dumpNodes(unprocessedNodes);
        throw new Error('Unable to process any further node');
      }

      nodes = unprocessedNodes;
    }
  }

  dumpNodes(nodes) {
    for (let node of nodes) {
      node.dump('', 0);
    }
  }
}
