export default class MemoryConditionJoiner {
  joinChildren(node, conditions=[]) {
    const children = node.getChildren();

    for (let child of children) {
      this.joinChild(child, conditions);
    }
  }

  join(node, conditions=[]) {
    var newConditions = [];

    if (!conditions.length) {
      newConditions = node.getAllowed().slice();
    } else {
      for (let location of node.getAllowed()) {
        for (let condition of conditions) {
          var newLocation = condition.merged(location);

          if (newLocation) {
            newConditions.push(newLocation);
          }
        }
      }
    }

    if (!newConditions.length) {
      throw new Error(`Could not join two location lists, because they excluded each other:\n\n${node._name}: ${node.getAllowed()}\nWITH: ${conditions}`);
    }

    return newConditions;
  }

  joinChild(node, conditions=[]) {
    const newConditions = this.join(node, conditions);
    node.setAllowed(newConditions);
    this.joinChildren(node, newConditions);
  }
}
