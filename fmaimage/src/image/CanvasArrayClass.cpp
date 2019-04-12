#include <image/CanvasArrayClass.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::image;

// ----------------------------------------------------------------------------
ClassPtr CanvasArrayClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("CanvasArray", "fmaImage::CanvasArray"));
  klass->extends(ClassObject);

  root->setMember("CanvasArray", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr CanvasArrayClass::createInstance(const interpret::ContextPtr &context, std::vector<types::TypePtr> &list) {
  ClassPtr number = context->getRootLevelContext()->getMember("fmaImage")->get()->getMember("CanvasArray")->asClass();
  if (!number) {
    return ResultPtr(new Result());
  }

  GroupedParameterList parameters;
  parameters.push_back(TypePtr(new InternalArrayValue(list)));
  return ResultPtr(new Result(context, number->createInstance(context, parameters)));
}
