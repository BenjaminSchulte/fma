#include <Project.hpp>
#include <Parser.hpp>
#include <AstNode.hpp>

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New<v8::String>("Project").ToLocalChecked(), Project::CREATE().ToLocalChecked());
  Nan::Set(target, Nan::New<v8::String>("Parser").ToLocalChecked(), Parser::CREATE().ToLocalChecked());
  Nan::Set(target, Nan::New<v8::String>("AstNode").ToLocalChecked(), AstNode::CREATE().ToLocalChecked());
}

NODE_MODULE(fma, InitAll)
