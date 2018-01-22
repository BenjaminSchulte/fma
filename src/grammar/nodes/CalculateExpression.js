import NestedExpressionNode from './NestedExpressionNode';

export default class CalculateExpression extends NestedExpressionNode {
  constructor(parent, operator, right) {
    super(parent);

    this.operator = operator;
    this.right = right;
  }

  serialize(s) {
    return [
      super.serialize(s),
      s.serializeString(this.operator),
      s.serialize(this.right)
    ]
  }

  deserialize(s, args) {
    super.deserialize(s, args[0]);
    this.operator = s.deserializeString(args[1]);
    this.right = s.deserialize(args[2]);
  }

  static deserialize(s, args) {
    const node = new CalculateExpression(null, null, null);
    node.deserialize(s, args);
    return node;
  }

  dump() {
    return `(${this.parent.dump()} ${this.operator} ${this.right.dump()})`;
  }

  asCallExpression(callback) {
    this.right.asCallExpression(callback);
    return this;
  }

  static build(c, left, calcs) {
    if (calcs.length === 0) {
      return left;
    }

    calcs = calcs.slice();
    const calc = calcs.shift();
    const result = CalculateExpression._build(
      c,
      c(new CalculateExpression(left, calc[0], calc[1])),
      calcs
    );

    //console.log(result.dump());

    return result;
  }

  static _build(c, left, calcs) {
    if (calcs.length === 0) {
      return left;
    }

    const calc = calcs.shift();
    if (!CalculateExpression.PRIORITY_INDEX.hasOwnProperty(left.operator)) {
      throw new Error('Priority for operator is undefined: ' + left.operator);
    }
    if (!CalculateExpression.PRIORITY_INDEX.hasOwnProperty(calc[0])) {
      throw new Error('Priority for operator is undefined: ' + calc[0]);
    }

    const myPrio = CalculateExpression.PRIORITY_INDEX[calc[0]];
    const theirPrio = CalculateExpression.PRIORITY_INDEX[left.operator];

    if (myPrio <= theirPrio) {
      //console.log(`${calc[0]}(${myPrio}) <= ${left.operator}(${theirPrio})`);
      //console.log('LEFT: ', left.dump());

      left = c(new CalculateExpression(left, calc[0], calc[1]));
      left = CalculateExpression._build(c, left, calcs);
    } else {
      //console.log(`${calc[0]}(${myPrio}) > ${left.operator}(${theirPrio})`);
      //console.log('LEFT: ', left.dump());

      var newOp = c(new CalculateExpression(left.right, calc[0], calc[1]));
      left.right = CalculateExpression._build(c, newOp, calcs);;
    }

    return left;
  };
}

const PRIORITY = [
  ['*', '/'],
  ['+', '-'],
  ['<<', '>>'],
  ['&'],
  ['|'],
  ['<', '>'],
  ['==', '!='],
  ['||', '&&'],
  ['=', '|=', '&=', '+=', '-=', '*=', '/='],
];

CalculateExpression.PRIORITY_INDEX = {};

for (let i=0; i<PRIORITY.length; i++) {
  for (let op of PRIORITY[i]) {
    CalculateExpression.PRIORITY_INDEX[op] = 1000 - i;
  }
}
