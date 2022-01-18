#ifndef __FMA_SYMBOL_SYMBOLREFERENCE_H__
#define __FMA_SYMBOL_SYMBOLREFERENCE_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class SymbolReference : public Reference {
public:
  SymbolReference(const std::string &name) : name(name) {}
  virtual ~SymbolReference() {}

  static std::string ReferenceTypeId() { return "FMA::symbol::SymbolReference"; }
  std::string referenceTypeId() const override { return ReferenceTypeId(); }

  virtual std::string asString() const override { return name; }
  inline const std::string &getName() const { return name; }

  virtual bool isSymbolReference() const override { return true; }

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const override;

  bool serialize(FMA::output::DynamicBuffer &) const override;

  static std::shared_ptr<SymbolReference> deserialize(Log *, FMA::output::DynamicBuffer &);
  
protected:
  std::string name;
};

typedef std::shared_ptr<SymbolReference> SymbolReferencePtr;

}
}

#endif
