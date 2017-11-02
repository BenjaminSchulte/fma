import * as Nodes from './nodes';
import CommentCollector from './CommentCollector';
import UndefinedObject from '../objects/Undefined';
import ChildValueAccessor from './ChildValueAccessor';
import ValueAccessor from './ValueAccessor';
import InterpreterError from './InterpreterError';

export default class Context {
  constructor(interpreter, object, parent=null) {
    if (!interpreter) {
      throw new InterpreterError('Missing parameter: interpreter');
    }

    this.interpreter = interpreter;
    this.comments = new CommentCollector();
    this.object = object;
    this.parent = parent;
    this.returnValue = new UndefinedObject();
  }

  withoutParents() {
    return new Context(this.getInterpreter(), this.object);
  }

  async resolveChild(name) {
    if (this.object.hasMember(name)) {
      return new ChildValueAccessor(this.object, this.object.getMember(name), name);
    }

    if (this.parent) {
      var parentNode = await this.parent.resolveChild(name);

      if (!parentNode.isUndefined()) {
        return parentNode;
      }
    }

    return new ChildValueAccessor(this.object, new UndefinedObject(), name);
  }

  enter(object) {
    return new Context(this.getInterpreter(), object, this);
  }

  injectParent(parent) {
    if (this.parent !== null) {
      throw new InterpreterError('injectParent does not yet support injecting parents in trees')
    }

    this.parent = parent;
  }

  getRoot() {
    return new Context(this.getInterpreter(), this.getInterpreter().root);
  }

  getInterpreter() {
    return this.interpreter;
  }

  getObject() {
    return this.object;
  }

  getProcessor(node) {
    const type = node.type();
    const Processor = Nodes[type];

    if (!Processor) {
      throw new InterpreterError(`Unhandled node type: ${type}`);
    }

    const instance = new Processor(this, node);
    return instance;
  }

  async process(node) {
    const instance = this.getProcessor(node);
    if (instance.setComments(this.comments)) {
      this.comments.reset();
    }

    try {
      return await instance.process();
    } catch(err) {
      if (!err.doNotShowError) {
        this.getInterpreter().log('error', node.getLocation().toString() + ': ' + err.toString());
        console.error(err);
        err = new Error('There was at least one unrecoverable error while compiling.');
        err.doNotShowError = true;
      }
      throw err;
    }
  }

  async processMany(nodes) {
    var result;

    for (let node of nodes) {
      result = await this.process(node);
    }

    if (!result) {
      result = new ValueAccessor(new UndefinedObject());
    }

    return result;
  }

  async resolve(node) {
    var result = await this.process(node);
    const object = result.getObject();

    if (!object.canBeCalled) {
      throw new Error('Invalid object result from ' + node.type());
    }

    if (object.canBeCalled()) {
      result = await object.callWithParameters(this);

      if (!result) {
        result = new ValueAccessor(new UndefinedObject());
      } else {
        result = new ValueAccessor(result);
      }
    }

    return result;
  }
}
