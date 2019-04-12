#include <fma/types/DecoratorContainer.hpp>
#include <fma/types/Decoratable.hpp>
#include <fma/types/Decorator.hpp>
#include <iostream>

using namespace FMA::types;
using namespace FMA::interpret;

// ----------------------------------------------------------------------------
DecoratorContainerPtr DecoratorContainer::createChild() {
  DecoratorContainerPtr child(new DecoratorContainer());

  child->parent = std::dynamic_pointer_cast<DecoratorContainer>(getPointer());

  return child;
}

// ----------------------------------------------------------------------------
void DecoratorContainer::attachDecorators(const DecoratablePtr &decoratable) {
  DecoratablePtr current(decoratable);

  // Find top most decoratable
  while (current->hasDecorator()) {
    TypePtr next = current->getDecorator();
    if (!next->isDecoratable()) {
      std::cerr << "Invalid decorator attached to object" << std::endl;
      break;
    }

    current = std::dynamic_pointer_cast<Decoratable>(next);
  }

  for (auto &dec : decorators) {
    auto newDec = dec->clone();

    current->setDecorator(newDec);
    current = newDec;
  }

  auto parentPtr = parent.lock();
  if (parentPtr) {
    parentPtr->attachDecorators(current);
  }
}

// ----------------------------------------------------------------------------

