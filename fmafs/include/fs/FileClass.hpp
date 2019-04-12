#ifndef __FMA_FS_FILECLASS_H__
#define __FMA_FS_FILECLASS_H__

#include <fma/types/Class.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/types/InternalValue.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>

namespace FMA {
namespace fs {

class FileClass : public types::Class {
public:
  static types::ClassPtr create(const types::ModulePtr &root, const types::ClassPtr &Class);

  // STATIC
  static interpret::ResultPtr directory_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr file_qm(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr mtime(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr write(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
  static interpret::ResultPtr read(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);

  // PROTOTYPE
  static interpret::ResultPtr initialize(const interpret::ContextPtr &context, const interpret::GroupedParameterList &parameter);
};

}
}

#endif
