#ifndef __FMA_TYPES_DECORATORCONTAINER_H__
#define __FMA_TYPES_DECORATORCONTAINER_H__

#include "Base.hpp"
#include <vector>

#define FMA_TYPE_DECORATORCONTAINER_MEMBER "@decoratorcontainer"

namespace FMA {
namespace types {

typedef std::vector<DecoratorPtr> DecoratorList;
typedef std::shared_ptr<class DecoratorContainer> DecoratorContainerPtr;
typedef std::weak_ptr<class DecoratorContainer> WeakDecoratorContainerPtr;

class DecoratorContainer : public Base {
public:
  DecoratorContainer() {}

  DecoratorContainerPtr createChild();

  std::string asString() const { return "<DecoratorContainer>"; };
  bool isDecoratorContainer() const { return true; }

  void registerCallback(const DecoratorPtr &callback) {
    decorators.push_back(callback);
  }

  void attachDecorators(const DecoratablePtr &decoratable);

protected:
  WeakDecoratorContainerPtr parent;
  DecoratorList decorators;
};

}
}

#endif
