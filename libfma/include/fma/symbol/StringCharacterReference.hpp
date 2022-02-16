#ifndef __FMA_SYMBOL_STRINGCHARACTERREFERENCE_H__
#define __FMA_SYMBOL_STRINGCHARACTERREFERENCE_H__

#include "Reference.hpp"

namespace FMA {
namespace symbol {

class StringCharacterReference : public Reference {
public:
  StringCharacterReference(const std::string &text, const ReferencePtr &index, uint8_t numBytes)
    : mText(text)
    , mIndex(index)
    , mNumBytes(numBytes)
  {}

  static std::string ReferenceTypeId() { return "FMA::symbol::StringCharacter"; }
  std::string referenceTypeId() const override { return ReferenceTypeId(); }

  virtual std::string asString() const override;
  virtual bool isConstant() const override;
  virtual uint64_t asConstant() const override;
  uint64_t asConstant(uint64_t index) const;

  virtual uint64_t resolve(const plugin::MemorySymbolMap *map, bool &valid) const override;

  bool serialize(FMA::output::DynamicBuffer &) const override;
  static std::shared_ptr<StringCharacterReference> deserialize(Log *, FMA::output::DynamicBuffer &);
  
protected:
  std::string mText;
  ReferencePtr mIndex;
  uint8_t mNumBytes;
};

typedef std::shared_ptr<StringCharacterReference> StringCharacterReferencePtr;

}
}

#endif
