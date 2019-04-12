#include <Parser.hpp>
#include <Project.hpp>
#include <AstNode.hpp>
#include <fma/Parser.hpp>
#include <fma/Project.hpp>

// ----------------------------------------------------------------------------
Parser::Parser(const v8::Local<v8::Object> &project)
  : project(project)
{
  FMA::Project* fmaProject = Nan::ObjectWrap::Unwrap<Project>(project)->getProject();
  parser = new FMA::Parser(fmaProject);
}

// ----------------------------------------------------------------------------
Parser::~Parser() {
  delete parser;
}

// ----------------------------------------------------------------------------
Nan::MaybeLocal<v8::Function> Parser::CREATE() {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "parseString", ParseString);
  Nan::SetPrototypeMethod(tpl, "parseFile", ParseFile);
  Nan::SetPrototypeMethod(tpl, "getRootNode", GetRootNode);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  return Nan::GetFunction(tpl);
}

// ----------------------------------------------------------------------------
NAN_METHOD(Parser::ParseString) {
  FMA::Parser *parser = Nan::ObjectWrap::Unwrap<Parser>(info.Holder())->parser;

  if (info.Length() < 1) {
    return Nan::ThrowError(Nan::New("Missing argument for parser").ToLocalChecked());
  }

  if (!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("First parameter must be string").ToLocalChecked());
  }

  v8::String::Utf8Value value(info[0]->ToString());
  bool result = parser->parseString(*value);

  info.GetReturnValue().Set(Nan::New(result));
}

// ----------------------------------------------------------------------------
NAN_METHOD(Parser::ParseFile) {
  FMA::Parser *parser = Nan::ObjectWrap::Unwrap<Parser>(info.Holder())->parser;

  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("Missing argument for parser").ToLocalChecked());
  }

  if (!info[0]->IsString()) {
    return Nan::ThrowError(Nan::New("First parameter must be string").ToLocalChecked());
  }

  v8::String::Utf8Value value(info[0]->ToString());
  bool result = parser->parseFile(*value);

  info.GetReturnValue().Set(Nan::New(result));
}

// ----------------------------------------------------------------------------
NAN_METHOD(Parser::GetRootNode) {
  FMA::Parser *parser = Nan::ObjectWrap::Unwrap<Parser>(info.Holder())->parser;

  v8::Local<v8::Function> cons = Nan::New(AstNode::constructor());
  v8::Local<v8::Object> object = Nan::NewInstance(cons, 0, NULL).ToLocalChecked();

  Nan::ObjectWrap::Unwrap<AstNode>(object)->set(parser->getResult());

  info.GetReturnValue().Set(object);
}

// ----------------------------------------------------------------------------
NAN_METHOD(Parser::New) {
  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("Missing argument for parser").ToLocalChecked());
  }

  if (!info[0]->IsObject()) {
    return Nan::ThrowError(Nan::New("First parameter must be object").ToLocalChecked());
  }

  v8::Local<v8::Object> project = info[0]->ToObject();
  Parser *obj = new Parser(project);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}
