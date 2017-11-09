import ObjectClass from './Object';

export default class ArrayObject extends ObjectClass {
  constructor(items=[]) {
    super();

    this.items = items;
  }

  getItems() {
    return this.items;
  }

  type() {
    return 'Array';
  }

  initializeClassMembers(klass) {
    super.initializeClassMembers(klass);

    klass.on('append', ['*args'], (self, args, context) => {
      for (let item of args.getItems()) {
        self.items.push(item);
      }
    });

    klass.on('length', [], (self, context) => {
      return context.returnNumber(self.items.length);
    });

    klass.on('each', ['&callback'], (self, block, context) => {
      var result = [];

      for (let item of self.items) {
        result.push(block.getMacro().callWithParameters(context.getContext(), item));
      }

      return new ArrayObject(result);
    })

    klass.on('map', ['&block'], (self, block, context) => {
      var result = [];

      for (let item of self.items) {
        result.push(block.getMacro().callWithParameters(context.getContext(), item));
      }

      return new ArrayObject(result);
    });

    klass.on('sort', [], (self, context) => {
      var items = self.items.slice();
      items = items.sort((a, b) => {
        if (a.hasMember('==')) {
          if (context.asBoolean(a.getMember('==').callWithParameters(context.getContext(), b))) {
            return 0;
          }
        } else if (b.hasMember('==')) {
          if (context.asBoolean(b.getMember('==').callWithParameters(context.getContext(), a))) {
            return 0;
          }
        }
        if (a.hasMember('<')) {
          return context.asBoolean(a.getMember('<').callWithParameters(context.getContext(), b)) ? -1 : 1;
        }
        if (a.hasMember('>')) {
          return context.asBoolean(a.getMember('>').callWithParameters(context.getContext(), b)) ? 1 : -1;
        }
        if (b.hasMember('<')) {
          return context.asBoolean(b.getMember('<').callWithParameters(context.getContext(), a)) ? 1 : -1;
        }
        if (b.hasMember('>')) {
          return context.asBoolean(b.getMember('>').callWithParameters(context.getContext(), a)) ? -1 : 1;
        }

        throw new Error('Can not sort ' + a.getClassName() + ' with ' + b.getClassName());
      });

      return new ArrayObject(items);
    });

  }
}
