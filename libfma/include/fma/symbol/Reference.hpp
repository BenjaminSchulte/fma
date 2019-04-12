#ifndef __FMA_SYMBOL_REFERENCE_H__
#define __FMA_SYMBOL_REFERENCE_H__

#include <memory>
#include <string>

namespace FMA {
namespace plugin {
class MemorySymbolMap;
}
namespace symbol {

class Reference {
public:
  Reference() {}
  virtual ~Reference() {}

  virtual std::string asString() const=0;
  virtual bool isConstant() const { return false; }
  virtual uint64_t asConstant() const { return 0; }

  virtual bool isSymbolReference() const { return false; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const = 0;
};

typedef std::shared_ptr<Reference> ReferencePtr;

}
}

#endif
