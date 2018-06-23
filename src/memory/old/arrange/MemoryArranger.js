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
    for (let node of fullResult) {
      if (result.indexOf(node) === -1) {
        result.push({score: node.ratePriority(), node});
      }
    }
    return result;
  }

  priorizeNodes(nodes) {
    return nodes.sort((a, b) => {
      const r = b.score - a.score;
      if (r !== 0) {
        return r;
      }

      return a.node.siblingIndex() - b.node.siblingIndex();
    })
  }

  arrange() {
    this.joinConditions();

    const processor = new MemoryNodeProcessor();
    var nodes = this.priorizeNodes(this.flattenNodes());
    while (nodes.length) {
      var numNodes = nodes.length;
      var unprocessedNodes = [];
      var lastScore = null;

      for (let i=0; i<numNodes; i++) {
        var {score, node} = nodes[i];

        if (lastScore !== null && lastScore > score) {
          lastScore = score;

          if (unprocessedNodes.length) {
            nodes = unprocessedNodes.concat(nodes.slice(i));
            unprocessedNodes = [];
            numNodes = nodes.length;
            i = -1;
            lastScore = null;
            continue;
          }
        }

        if (!processor.process(node)) {
          unprocessedNodes.push(node);
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
