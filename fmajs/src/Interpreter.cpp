#include <Interpreter.hpp>
#include <Project.hpp>
#include <AstNode.hpp>
#include <fma/interpret/Interpreter.hpp>
#include <fma/Project.hpp>

// ----------------------------------------------------------------------------
Interpreter::Interpreter(const v8::Local<v8::Object> &project)
  : project(project)
{
  FMA::Project* fmaProject = Nan::ObjectWrap::Unwrap<Project>(project)->getProject();
  interpreter = new FMA::interpret::Interpreter(fmaProject);
}

// ----------------------------------------------------------------------------
Interpreter::~Interpreter() {
  delete interpreter;
}

// ----------------------------------------------------------------------------
Nan::MaybeLocal<v8::Function> Interpreter::CREATE() {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Interpreter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  return Nan::GetFunction(tpl);
}

// ----------------------------------------------------------------------------
NAN_METHOD(Interpreter::New) {
  if (info.Length() != 1) {
    return Nan::ThrowError(Nan::New("Missing argument for interpreter").ToLocalChecked());
  }

  if (!info[0]->IsObject()) {
    return Nan::ThrowError(Nan::New("First parameter must be object").ToLocalChecked());
  }

  v8::Local<v8::Object> project = info[0]->ToObject();
  Interpreter *obj = new Interpreter(project);
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}
