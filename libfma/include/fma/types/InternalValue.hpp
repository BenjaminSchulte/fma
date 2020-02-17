#ifndef __FMA_TYPES_INTERNALVALUE_H__
#define __FMA_TYPES_INTERNALVALUE_H__

#include <string>
#include <functional>
#include <sstream>
#include <vector>
#include <map>
#include "Decoratable.hpp"

namespace FMA {
namespace output {
typedef std::shared_ptr<class DynamicBuffer> DynamicBufferPtr;
}
namespace serialize {
typedef std::weak_ptr<class SerializeObject> WeakSerializeObjectPtr;
typedef std::weak_ptr<class DeserializeObject> WeakDeserializeObjectPtr;
}
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;
struct GroupedParameterList;
}
namespace symbol {
typedef std::shared_ptr<class Reference> ReferencePtr;
}
namespace types {

class InternalValue : public Base {
public:
protected:
};

template<class T> const char *getInternalObjectName();

template<class T>
class InternalObject : public InternalValue {
public:
  InternalObject() : value() {}
  InternalObject(const T& value) : value(value) {}

  virtual bool isInternalObject() const { return true; }
  virtual bool isInternalObjectOfType(const std::string &type) const {
    return type == getTypeName();
  }

  virtual const char *getTypeName() const { return getInternalObjectName<T>(); };

  virtual T &getValue() { return value; }
  virtual std::string asString() const {
    std::ostringstream os;
    os << "<Internal" << getTypeName() << ">";
    return os.str();
  }

protected:
  T value;
};

typedef std::function<interpret::ResultPtr(const interpret::ContextPtr&, const interpret::GroupedParameterList &)> InternalFunction;

typedef InternalObject<std::vector<TypePtr> > InternalArrayValue;
typedef InternalObject<std::map<std::string, TypePtr> > InternalMapValue;
typedef InternalObject<std::string> InternalStringValue;
typedef InternalObject<long double> InternalNumberValue;
typedef InternalObject<symbol::ReferencePtr> InternalReferenceValue;
typedef InternalObject<output::DynamicBufferPtr> InternalBufferValue;
typedef InternalObject<serialize::WeakSerializeObjectPtr> InternalSerializeObjectValue;
typedef InternalObject<serialize::WeakDeserializeObjectPtr> InternalDeserializeObjectValue;


struct InternalRange {
  InternalRange() : first(0), last(0) {}
  InternalRange(int64_t first, int64_t last) : first(first), last(last) {}

  int64_t first;
  int64_t last;
};

class InternalFunctionValue : public Decoratable {
public:
  InternalFunctionValue(const std::string &name, const InternalFunction &func, DecoratorCallType decorator=DECORATORCALL_NONE) : name(name), func(func), decoratorType(decorator) {};

  virtual interpret::ResultPtr callInDecorator(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, DecoratorCallType type); 
  virtual std::string asString() const;
  virtual DecoratorCallType getDecoratorCallType() const { return decoratorType; }

protected:
  std::string name;
  InternalFunction func;
  DecoratorCallType decoratorType;
};

}
}

#endif
