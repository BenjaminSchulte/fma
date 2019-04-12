#ifndef __FMA_INTERPRET_CALLSTACK_H__
#define __FMA_INTERPRET_CALLSTACK_H__

#include <vector>
#include <memory>
#include <fma/Reference.hpp>

namespace FMA {
namespace interpret {

struct CallStackItem {
  CallStackItem(const CodeReference &ref) : ref(ref) {}

  CodeReference ref;
};

typedef std::vector<CallStackItem> CallStackItemList;

class CallStack;

struct CallStackContext {
  CallStackContext(CallStack &stack) : stack(stack) {}
  ~CallStackContext();

protected:
  CallStack &stack;
};

typedef std::shared_ptr<CallStackContext> CallStackContextPtr;

class CallStack {
public:

  CallStack() {}
  ~CallStack() {}

  inline CallStackContextPtr enter(const CodeReference &ref) {
    stack.push_back(CallStackItem(ref));
    return CallStackContextPtr(new CallStackContext(*this));
  }

  inline void leave() {
    stack.pop_back();
  }

  inline CallStackItemList &all() { return stack; }

protected:
  CallStackItemList stack;
};

}
}

#endif
