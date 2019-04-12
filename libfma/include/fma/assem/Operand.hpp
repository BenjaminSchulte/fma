#ifndef __FMA_ASM_OPERAND_H__
#define __FMA_ASM_OPERAND_H__

#include <string>
#include <memory>

namespace FMA {
namespace symbol {
typedef std::shared_ptr<class Reference> ReferencePtr;
}
namespace assem {

class Operand {
public:
  Operand();
  virtual ~Operand();

  virtual std::string asString() const = 0;
  virtual std::string getIdentifier() = 0;

  virtual bool isConstant() const { return false; }
  virtual uint64_t asConstant() const { return 0; }

  virtual bool isResolvedAddress() const { return false; }
  virtual uint64_t asResolvedAddress() const { return 0; }

  virtual bool isSymbolReference() const { return false; }
  virtual symbol::ReferencePtr asSymbolReference() const { return symbol::ReferencePtr(); }

  virtual bool isWriteable() = 0;
  virtual bool isReadable() = 0;
};

}
}

#endif