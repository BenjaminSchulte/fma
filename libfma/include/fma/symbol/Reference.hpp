#ifndef __FMA_SYMBOL_REFERENCE_H__
#define __FMA_SYMBOL_REFERENCE_H__

#include <memory>
#include <string>

namespace FMA {
class Log;
namespace output {
class DynamicBuffer;
}

namespace plugin {
class MemorySymbolMap;
}
namespace symbol {

enum class SerializeReferenceId {
  SYMBOL_REFERENCE,
  CALCULATE_REFERENCE,
  CONST8_REFERENCE,
  CONST16_REFERENCE,
  CONST32_REFERENCE,
  CONST64_REFERENCE,
  SIGNED_ASSET_RANGE_REFERENCE,
};

class Reference {
public:
  Reference() {}
  virtual ~Reference() {}

  virtual std::string asString() const=0;
  virtual bool isConstant() const { return false; }
  virtual uint64_t asConstant() const { return 0; }

  virtual bool isSymbolReference() const { return false; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const = 0;

  virtual bool serialize(FMA::output::DynamicBuffer &) const = 0;

  static std::shared_ptr<Reference> deserialize(Log *, FMA::output::DynamicBuffer &);
};

typedef std::shared_ptr<Reference> ReferencePtr;

}
}

#endif
