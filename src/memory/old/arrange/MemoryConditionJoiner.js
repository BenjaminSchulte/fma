export default class MemoryConditionJoiner {
  joinChildren(node, conditions=[]) {
    const children = node.getChildren();

    for (let child of children) {
      this.joinChild(child, conditions);
    }
  }

  joinChild(node, conditions=[]) {
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

    node.setAllowed(newConditions);

    this.joinChildren(node, newConditions);
  }
}
