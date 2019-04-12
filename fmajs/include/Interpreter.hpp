#ifndef __FMAJS_INTERPRETER_H__
#define __FMAJS_INTERPRETER_H__ 

#include <nan.h>

namespace FMA {
  namespace interpret {
    class Interpreter;
  }
}

class Interpreter : public Nan::ObjectWrap {
public:
  static Nan::MaybeLocal<v8::Function> CREATE();

protected:
  explicit Interpreter(const v8::Local<v8::Object> &project);
  ~Interpreter();

  static NAN_METHOD(New);

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  FMA::interpret::Interpreter *interpreter;
  v8::Local<v8::Object> project;
};

#endif
