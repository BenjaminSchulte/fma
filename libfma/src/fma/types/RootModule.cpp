#include <fma/types/RootModule.hpp>
#include <fma/types/DecoratorContainer.hpp>
#include <iostream>

using namespace FMA;
using namespace FMA::types;

// ----------------------------------------------------------------------------
RootModule::RootModule()
  : Module("")
{
  setMember(FMA_TYPE_DECORATORCONTAINER_MEMBER, TypePtr(new DecoratorContainer()));
}

// ----------------------------------------------------------------------------
RootModule::~RootModule() {
}

// ----------------------------------------------------------------------------

