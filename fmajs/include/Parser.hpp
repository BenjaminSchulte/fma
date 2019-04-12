#ifndef __FMAJS_PARSER_H__
#define __FMAJS_PARSER_H__ 

#include <nan.h>

namespace FMA {
  class Parser;
}

class Parser : public Nan::ObjectWrap {
public:
  static Nan::MaybeLocal<v8::Function> CREATE();

protected:
  explicit Parser(const v8::Local<v8::Object> &project);
  ~Parser();

  static NAN_METHOD(New);

  static NAN_METHOD(ParseString);
  static NAN_METHOD(ParseFile);
  static NAN_METHOD(GetRootNode);

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  FMA::Parser *parser;
  v8::Local<v8::Object> project;
};

#endif
