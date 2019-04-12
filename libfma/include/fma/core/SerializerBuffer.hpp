#ifndef __FMA_CORE_SERIALIZERBUFFER_H__
#define __FMA_CORE_SERIALIZERBUFFER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/Log.hpp>

namespace FMA {
class Project;

namespace core {

class SerializerBufferClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const serialize::WeakSerializeObjectPtr &object);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr size(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr db(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dw(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dd(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr object(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);


  static interpret::ResultPtr _writeNumber(const interpret::ContextPtr &context, uint8_t numBytes, const interpret::GroupedParameterList &parameter);

  static const serialize::WeakSerializeObjectPtr &getBuffer(const interpret::ContextPtr &context);
  static const serialize::WeakSerializeObjectPtr &getBuffer(Project *project, const types::TypePtr &value);
};

}
}

#endif
