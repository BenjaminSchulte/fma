#include <fma/symbol/StringCharacterReference.hpp>
#include <fma/output/DynamicBuffer.hpp>
#include <sstream>

using namespace FMA::symbol;

// ----------------------------------------------------------------------------
std::string StringCharacterReference::asString() const {
  std::ostringstream os;
  os << mText << "[" << mIndex->asString() << "]";
  return os.str();
}

// ----------------------------------------------------------------------------
bool StringCharacterReference::isConstant() const {
  return mIndex->isConstant();
}

// ----------------------------------------------------------------------------
uint64_t StringCharacterReference::asConstant(uint64_t index) const {
  uint64_t len = mText.length();
  if (len == 0) {
    return 0;
  }

  uint64_t value = 0;
  for (uint8_t i=0; i<mNumBytes; i++) {
    value |= mText[(index + i) % len] << (i * 8);
  }
  return value;
}

// ----------------------------------------------------------------------------
uint64_t StringCharacterReference::asConstant() const {
  return asConstant(mIndex->asConstant());
}

// ----------------------------------------------------------------------------
uint64_t StringCharacterReference::resolve(const plugin::MemorySymbolMap *map, bool &valid) const {
  uint64_t index = mIndex->resolve(map, valid);
  if (!valid) {
    return 0;
  }

  return asConstant(index);
}

// ----------------------------------------------------------------------------
bool StringCharacterReference::serialize(FMA::output::DynamicBuffer &buffer) const {
  uint16_t id = (uint16_t)SerializeReferenceId::STRING_CHARACTER_REFERENCE;

  buffer.write(&id, 2);
  buffer.writeNumber(mText.length(), 2);
  buffer.write(mText.c_str(), mText.size());
  if (!mIndex->serialize(buffer)) {
    return false;
  }

  buffer.writeNumber(mNumBytes, 1);
  return true;
}

// ----------------------------------------------------------------------------
StringCharacterReferencePtr StringCharacterReference::deserialize(Log *log, FMA::output::DynamicBuffer &buffer) {
  uint16_t textLength;
  buffer.read(&textLength, 2);

  char *textbuffer = (char*)malloc(textLength + 1);
  textbuffer[textLength] = 0;
  buffer.read(textbuffer, textLength);
  std::string text(textbuffer);
  free(textbuffer);

  ReferencePtr index(Reference::deserialize(log, buffer));
  uint8_t numBytes;
  buffer.read(&numBytes, 1);

  return StringCharacterReferencePtr(new StringCharacterReference(text, index, numBytes));
}
