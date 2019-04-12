#ifndef __FMA_CORE_DESERIALIZERBUFFER_H__
#define __FMA_CORE_DESERIALIZERBUFFER_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/serialize/DeserializeObject.hpp>
#include <fma/Log.hpp>

namespace FMA {
class Project;

namespace core {

class DeserializerBufferClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const serialize::WeakDeserializeObjectPtr &object);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr size(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr store(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr db(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dw(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dd(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr object(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr _readNumber(const interpret::ContextPtr &context, uint8_t numBytes, const interpret::GroupedParameterList &parameter);

  static const serialize::WeakDeserializeObjectPtr &getBuffer(const interpret::ContextPtr &context);
  static const serialize::WeakDeserializeObjectPtr &getBuffer(Project *project, const types::TypePtr &value);
};

}
}

#endif
