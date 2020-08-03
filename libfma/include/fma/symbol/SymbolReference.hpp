#ifndef __FMA_SYMBOL_SYMBOLREFERENCE_H__
#define __FMA_SYMBOL_SYMBOLREFERENCE_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class SymbolReference : public Reference {
public:
  SymbolReference(const std::string &name) : name(name) {}
  virtual ~SymbolReference() {}

  virtual std::string asString() const { return name; }
  inline const std::string &getName() const { return name; }

  virtual bool isSymbolReference() const { return true; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const;

  bool serialize(FMA::output::DynamicBuffer &) const override;

  static std::shared_ptr<SymbolReference> deserialize(Log *, FMA::output::DynamicBuffer &);
  
protected:
  std::string name;
};

typedef std::shared_ptr<SymbolReference> SymbolReferencePtr;

}
}

#endif
