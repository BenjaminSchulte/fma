#include <fma/types/Base.hpp>
#include <fma/types/Object.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <fma/types/Decorator.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/StaticMemberContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/InstanceContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/serialize/SerializeObject.hpp>
#include <fma/Project.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::interpret;
using namespace FMA::types;
using namespace FMA::serialize;

// ----------------------------------------------------------------------------
Base::Base() {
}

// ----------------------------------------------------------------------------
Base::~Base() {
}

// ----------------------------------------------------------------------------
void Base::unload() {
  auto membersCopy = members;
  members.clear();

  for (auto &member : membersCopy) {
    member.second->unload();
  }
}

// ----------------------------------------------------------------------------
bool Base::isUndefined() const {
  return false;
}

// ----------------------------------------------------------------------------
bool Base::hasMember(const std::string &name) const {
  return members.find(name) != members.end();
}

// ----------------------------------------------------------------------------
TypePtr Base::getDirectMember(const std::string &name) const {
  if (hasMember("." + name)) {
    return getMember("." + name);
  } else {
    return getMember(name);
  }
}

// ----------------------------------------------------------------------------
TypePtr Base::getMember(const std::string &name) const {
  MemberMapConstIterator iterator = members.find(name);

  if (iterator == members.end()) {
    return undefinedValue;
  }

# ifdef DEBUG_FMA
    std::cout << "\x1b[30;1mGET " << asString() << "." << name << " (" << iterator->second->asString() << ")\x1b[m" << std::endl;
# endif

  return iterator->second;
}

// ----------------------------------------------------------------------------
void Base::setMember(const std::string &name, const TypePtr &value) {
  if (value->isUndefined()) {
    removeMember(name);
  } else {
#   ifdef DEBUG_FMA
      std::cout << "\x1b[30;1mSET " << asString() << "." << name << " = " << value->asString() << "\x1b[m" << std::endl;
#   endif

    members[name] = value;
  }
}

// ----------------------------------------------------------------------------
void Base::removeMember(const std::string &name) {
  MemberMapConstIterator iterator = members.find(name);
  if (iterator == members.end()) {
    return;
  }

  members.erase(iterator);
}

// ----------------------------------------------------------------------------
ResultPtr Base::call(const ContextPtr &context, const GroupedParameterList &parameter) {
  if (getDecoratorCallType() == DECORATORCALL_NONE) {
    return callWithoutDecoratorTest(context, parameter);
  }

  TypePtr container;
  if (context->hasCallee()) {
    container = context->getCallee()->getMember(FMA_TYPE_DECORATORCONTAINER_MEMBER);
  } else {
    ResultPtr result = context->resolve(FMA_TYPE_DECORATORCONTAINER_MEMBER);

    if (result->isUndefined()) {
      context->log().warn() << "Unable to access decorator container to attach decorator in " << context->asString();
      return ResultPtr(new Result());
    }
  
    container = result->get();
  }

  if (!container->isDecoratorContainer()) {
    context->log().error() << "Decorator container object has wrong type";
    return ResultPtr(new Result());
  }
  
  DecoratorContainerPtr containerPtr(std::dynamic_pointer_cast<DecoratorContainer>(container));
  containerPtr->registerCallback(DecoratorPtr(new Decorator(getPointer(), parameter)));

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr Base::callWithoutDecoratorTest(const ContextPtr& context, const GroupedParameterList&) {
# ifdef DEBUG_FMA
    std::cout << "Unhandled call to " << asString() << std::endl;
# endif
  return Result::executed(context, getPointer());
}

// ----------------------------------------------------------------------------
ResultPtr Base::callDirect(const std::string &name, const ContextPtr& context, const GroupedParameterList &parameter) {
  const TypePtr &callback = getMember(name);
  if (callback->isUndefined() || !(isObject() || isClass())) {
    context->log().error() << "Call to undefined function " << name << " in context " << context->asString();
    return ResultPtr(new Result());
  }

  if (isObject()) {
    ContextPtr callContext(new InstanceContext(context->getInterpreter(), asObject(), name));
    return ResultPtr(callback->call(callContext, parameter));
  } else if (isClass()) {
    ContextPtr callContext(new StaticMemberContext(context->getInterpreter(), asClass(), name));
    return ResultPtr(callback->call(callContext, parameter));
  } else {
    return ResultPtr(new Result());
  }
}

// ----------------------------------------------------------------------------
InternalRange Base::convertToRange(const ContextPtr &context) {
  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  if (!isObjectOfType("Range")) {
    return InternalRange();
  }

  return InternalRange(
    (int64_t)getMember("__first")->convertToNumber(context),
    (int64_t)getMember("__last")->convertToNumber(context)
  );
}

// ----------------------------------------------------------------------------
std::string Base::convertToString(const ContextPtr &context) {
  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  TypePtr stringObject = getMember("to_s")->call(objectContext, parameter)->asObjectOfType("String");
  if (!stringObject) {
    context->log().error() << "Could not call to_s on object " << asString();
    return "";
  }

  TypePtr valueObject = stringObject->getMember("__value");
  if (!valueObject->isInternalObjectOfType("String")) {
    context->log().error() << asString() << ".to_s did not return a string";
    return "";
  }

  InternalStringValue *value = dynamic_cast<InternalStringValue*>(valueObject.get());
  return value->getValue();
}

// ----------------------------------------------------------------------------
std::vector<TypePtr> Base::convertToArray(const ContextPtr &context) {
  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  if (!isObjectOfType("Array")) {
    return std::vector<TypePtr>();
  }

  TypePtr valueObject = getMember("__value");
  if (!valueObject->isInternalObjectOfType("Array")) {
    context->log().error() << asString() << " is not not an array";
    return std::vector<TypePtr>();
  }

  InternalArrayValue *value = dynamic_cast<InternalArrayValue*>(valueObject.get());
  return value->getValue();
}

// ----------------------------------------------------------------------------
std::map<std::string, TypePtr> Base::convertToMap(const ContextPtr &context) {
  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  if (!isObjectOfType("Map")) {
    return std::map<std::string, TypePtr>();
  }

  TypePtr valueObject = getMember("__value");
  if (!valueObject->isInternalObjectOfType("Map")) {
    context->log().error() << asString() << " is not not a map";
    return std::map<std::string, TypePtr>();
  }

  InternalMapValue *value = dynamic_cast<InternalMapValue*>(valueObject.get());
  return value->getValue();
}

// ----------------------------------------------------------------------------
long double Base::convertToNumber(const ContextPtr &context) {
  if (isInternalObjectOfType("Number")) {
    InternalNumberValue *value = dynamic_cast<InternalNumberValue*>(this);
    return value->getValue();
  }

  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  TypePtr stringObject = getMember("to_n")->call(objectContext, parameter)->asObjectOfType("Number");
  if (!stringObject) {
    context->log().error() << "Could not call to_n on object " << asString();
    return 0;
  }

  TypePtr valueObject = stringObject->getMember("__value");
  if (!valueObject->isInternalObjectOfType("Number")) {
    context->log().error() << asString() << ".to_n did not return a number";
    return 0;
  }

  InternalNumberValue *value = dynamic_cast<InternalNumberValue*>(valueObject.get());
  return value->getValue();
}

// ----------------------------------------------------------------------------
bool Base::convertToBoolean(const ContextPtr &context) {
  ContextPtr objectContext(new ObjectContext(context->getInterpreter(), getPointer()));
  interpret::GroupedParameterList parameter;

  TypePtr stringObject = getMember("to_b")->call(objectContext, parameter)->asObjectOfType("Boolean");
  if (!stringObject) {
    context->log().error() << "Could not call to_b on object " << asString();
    return false;
  }

  TypePtr valueObject = stringObject->getMember("__value");
  if (!valueObject->isInternalObjectOfType("Number")) {
    context->log().error() << asString() << ".to_b did not return a boolean";
    return false;
  }

  InternalNumberValue *value = dynamic_cast<InternalNumberValue*>(valueObject.get());
  return !!value->getValue();
}

// ----------------------------------------------------------------------------
void Base::dump(const std::string &prefix) {
  std::cout << prefix << asString() << std::endl;
}

// ----------------------------------------------------------------------------
SerializeType Base::getSerializeTypeId() const {
  return TYPE_INVALID;
}

// ----------------------------------------------------------------------------
void Base::serialize(const SerializeObjectPtr &object) {
  object->getContext()->log().error() << "Unable to serialize object: " << asString();
}

// ----------------------------------------------------------------------------



