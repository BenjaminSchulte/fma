#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>

#include <fs/FileClass.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Buffer.hpp>
#include <fma/core/Number.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/Object.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::fs;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr FileClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("File", "fs::File"));
  klass->extends(ClassObject);

  klass->setMember("directory?", TypePtr(new InternalFunctionValue("directory?", FileClass::directory_qm)));
  klass->setMember("file?", TypePtr(new InternalFunctionValue("file?", FileClass::file_qm)));
  klass->setMember("mtime", TypePtr(new InternalFunctionValue("mtime", FileClass::mtime)));
  klass->setMember("write", TypePtr(new InternalFunctionValue("write", FileClass::write)));
  klass->setMember("read", TypePtr(new InternalFunctionValue("read", FileClass::read)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", FileClass::initialize)));

  root->setMember("File", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Can not instanciate class 'File'";

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::directory_qm(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, boost::filesystem::is_directory(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::file_qm(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return BooleanClass::createInstance(context, false);
  }

  return BooleanClass::createInstance(context, boost::filesystem::is_regular_file(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::mtime(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    return NumberClass::createInstance(context, 0);
  }

  return NumberClass::createInstance(context, boost::filesystem::last_write_time(args.front()->convertToString(context)));
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::read(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (!args.size()) {
    context->log().error() << "Missing argument: filename";
    return ResultPtr(new Result());
  }

  std::string fileName= args.front()->convertToString(context);
  std::fstream fh = std::fstream(fileName, std::ios::in | std::ios::binary);
  if (!fh) {
    context->log().error() << "Could not open file: " << fileName;
    return ResultPtr(new Result());
  }

  fh.seekg(0, fh.end);
  uint64_t length = fh.tellg();
  fh.seekg(0, fh.beg);

  char buffer[1024];
  output::DynamicBufferPtr result(new output::DynamicBuffer());
  while (length) {
    uint16_t thisRead = std::min<uint64_t>(length, 1024);
    fh.read(buffer, thisRead);
    
    if (fh) {
      result->write(buffer, thisRead);
      length -= thisRead;
    } else {
      result->write(buffer, fh.gcount());
      length = 0;
    }
  }

  result->seek(0);

  TypePtr bufferObject = BufferClass::createInstance(context)->get();
  bufferObject->setMember("__buffer", TypePtr(new InternalBufferValue(result)));
  return Result::executed(context, bufferObject);
}

// ----------------------------------------------------------------------------
ResultPtr FileClass::write(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  if (args.size() != 2) {
    return ResultPtr(new Result());
  }

  const TypePtr &value = args.back();

  std::string fileName = args.front()->convertToString(context);
  std::fstream fh = std::fstream(fileName, std::ios::out | std::ios::binary);
  if (!fh) {
    context->log().error() << "Could not open file: " << fileName;
    return ResultPtr(new Result());
  }

  if (value->isObjectOfType("Buffer")) {
    const auto &buf = BufferClass::getBuffer(context->getProject(), value);
    if (buf) {
      fh.write((char*)buf->getData(), buf->getSize());
    }
  } else {
    fh << value->convertToString(context);
  }
  fh.close();

  return ResultPtr(new Result());
}
