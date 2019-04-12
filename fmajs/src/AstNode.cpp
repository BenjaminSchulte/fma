#include <AstNode.hpp>

// ----------------------------------------------------------------------------
AstNode::AstNode() {
}

// ----------------------------------------------------------------------------
AstNode::~AstNode() {
}

// ----------------------------------------------------------------------------
Nan::MaybeLocal<v8::Function> AstNode::CREATE() {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("AstNode").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  return Nan::GetFunction(tpl);
}

// ----------------------------------------------------------------------------
NAN_METHOD(AstNode::New) {
  AstNode *obj = new AstNode();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}
