#ifndef __FMAJS_ASTNODE_H__
#define __FMAJS_ASTNODE_H__ 

#include <nan.h>
#include <memory>

namespace FMA {
namespace ast {
  typedef std::shared_ptr<class Node> NodePtr;
}
}

class AstNode : public Nan::ObjectWrap {
public:
  static Nan::MaybeLocal<v8::Function> CREATE();

  inline void set(const FMA::ast::NodePtr &newNode) {
    node = newNode;
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

protected:
  explicit AstNode();
  ~AstNode();

  static NAN_METHOD(New);

  FMA::ast::NodePtr node;
};

#endif
