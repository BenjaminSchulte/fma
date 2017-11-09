import * as Nodes from './nodes';
import CommentCollector from './CommentCollector';
import UndefinedObject from '../objects/Undefined';
import NilObject from '../objects/Nil';
import ChildValueAccessor from './ChildValueAccessor';
import ValueAccessor from './ValueAccessor';
import InterpreterError from './InterpreterError';
import CallStack from './CallStack';

export default class Context {
  constructor(interpreter, object, parent=null) {
    if (!interpreter) {
      throw new InterpreterError('Missing parameter: interpreter');
    }

    this.interpreter = interpreter;
    this.comments = new CommentCollector();
    this.stack = new CallStack();
    this.object = object;
    this.parent = parent;
    this.returnValue = new NilObject();
  }

  clone() {
    const context = new Context(this.interpreter, this.object);
    context.parent = this.parent ? this.parent.clone() : null;
    context.stack = this.stack;
    return context;
  }

  withoutParents() {
    const context = new Context(this.getInterpreter(), this.object);
    context.stack = this.stack;
    return context;
  }

  resolveChild(name) {
    if (this.object.hasMember(name)) {
      return new ChildValueAccessor(this.object, this.object.getMember(name), name);
    }

    if (this.parent) {
      var parentNode = this.parent.resolveChild(name);

      if (!parentNode.isUndefined()) {
        return parentNode;
      }
    }

    return new ChildValueAccessor(this.object, new UndefinedObject(), name);
  }

  enter(object) {
    const context = new Context(this.getInterpreter(), object, this);
    context.stack = this.stack;
    return context;
  }

  injectParent(parent) {
    if (this.parent === null) {
      this.parent = parent;
      return this;
    }

    var context = this.clone();
    context.getTreeRoot().parent = parent;

    return context;
  }

  tree() {
    return (this.parent ? this.parent.tree() : []).concat([this]);
  }

  getNames() {
    return this.tree().map(context => context.object.getFullName());
  }

  getTreeRoot() {
    return this.parent ? this.parent.getTreeRoot() : this;
  }

  getRoot() {
    const context = new Context(this.getInterpreter(), this.getInterpreter().root);
    context.stack = this.stack;
    return context;
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

  process(node) {
    const instance = this.getProcessor(node);
    if (instance.setComments(this.comments)) {
      this.comments.reset();
    }

    try {
      return instance.process();
    } catch(err) {
      if (!err.doNotShowError) {
        this.getInterpreter().log('error', node.getLocation().toString() + ': ' + err.toString());
        for (let location of this.stack.getLocations().reverse()) {
          this.getInterpreter().log('error', 'in: ' + location);
        }
        console.error(err);
        err = new Error('There was at least one unrecoverable error while compiling.');
        err.doNotShowError = true;
      }
      throw err;
    }
  }

  callWithParameters(node, object, ...args) {
    this.stack.push(node);
    const result = object.callWithParameters(this, ...args);
    this.stack.pop();
    return result;
  }

  preprocessMany(nodes) {
    for (let node of nodes) {
      this.getProcessor(node).preprocess(node);
    }
  }

  processMany(nodes) {
    var result;

    for (let node of nodes) {
      result = this.process(node);
    }

    if (!result) {
      result = new ValueAccessor(new NilObject());
    }

    return result;
  }

  resolve(node) {
    var result = this.process(node);
    const object = result.getObject();

    if (!object.canBeCalled) {
      throw new Error('Invalid object result from ' + node.type());
    }

    if (object.canBeCalled()) {
      result = object.callWithParameters(this);

      if (!result) {
        result = new ValueAccessor(new NilObject());
      } else {
        result = new ValueAccessor(result);
      }
    }

    return result;
  }
}
