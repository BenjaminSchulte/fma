#ifndef __FMA_CORE_DATABLOCK_H__
#define __FMA_CORE_DATABLOCK_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>

namespace FMA {
class Project;
namespace plugin {
  class MemoryBlock;
}
namespace core {

class DataBlockClass : public types::Class {
public:
  static types::ClassPtr create(const types::RootModulePtr &root, const types::ClassPtr &Class);
  static interpret::ResultPtr createInstance(const interpret::ContextPtr &context, const types::MacroPtr &macro);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_call(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr op_call_direct(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr __current_block(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_n(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_b(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_sym(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr to_s(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  static interpret::ResultPtr override_located_at(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr allow(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr deny(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr db(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dw(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr dd(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr label(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // ACCESS
  static plugin::MemoryBlock* memoryBlock(const interpret::ContextPtr &context);
  static plugin::MemoryBlock* memoryBlock(const Project *project, const types::TypePtr &context);

protected:
  static interpret::ResultPtr writeData(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter, uint8_t itemSize);
  static void writeDataRecursive(plugin::MemoryBlock *block, const interpret::ContextPtr &context, const types::TypePtr &value, uint8_t itemSize, uint32_t &maxBytes);
  static types::ObjectPtr convertToWriteableType(const interpret::ContextPtr &context, const types::TypePtr &value);
};

}
}

#endif
