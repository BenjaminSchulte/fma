#include <fma/symbol/SymbolReference.hpp>
#include <fma/symbol/CalculatedNumber.hpp>
#include <fma/assem/RelativeAddressOperand.hpp>
#include <fma/assem/SymbolReferenceOperand.hpp>

#include <binary/CodeSection.hpp>
#include <iostream>

using namespace FMACPP::binary;

// ----------------------------------------------------------------------------
CodeSection::CodeSection(int id, const std::string &label, FMA::linker::LinkerBlock *block)
  : mId(id)
  , mLabel(label)
  , mBlock(block)
{
  mBlock->symbol(mLabel);
}

// ----------------------------------------------------------------------------
void CodeSection::markAsCode() {
  if (mHasCode) {
    return;
  }

  // Ensures that this code is being places somewhere
  mBlock->writeNumber(1, 1);
  mHasCode = true;
}

// ----------------------------------------------------------------------------
void CodeSection::writeCode(const std::string &line) {
  markAsCode();

  mCode.push_back(line);
}

// ----------------------------------------------------------------------------
void CodeSection::writeCode(const FMA::symbol::SymbolReferencePtr &ref) {
  mCode.push_back(ref);
}

// ----------------------------------------------------------------------------
void CodeSection::writeCode(const FMA::symbol::ReferencePtr &ref) {
  if (ref->isSymbolReference()) {
    FMA::symbol::SymbolReferencePtr sym(std::dynamic_pointer_cast<FMA::symbol::SymbolReference>(ref));
    mCode.push_back(sym);
    return;
  } else if (ref->isConstant()) {
    mCode.push_back(std::to_string(ref->asConstant()));
    return;
  }

  std::string typeId = ref->referenceTypeId();
  if (typeId == FMA::symbol::CalculatedNumber::ReferenceTypeId()) {
    std::shared_ptr<FMA::symbol::CalculatedNumber> typedRef(std::dynamic_pointer_cast<FMA::symbol::CalculatedNumber>(ref));
    writeCode("(");
    writeCode(typedRef->getLeft());
    writeCode(typedRef->operandAsString());
    writeCode(typedRef->getRight());
    writeCode(")");
    return;
  }

  std::cerr << "Unsupported reference type: " << typeId << std::endl;

  mCode.push_back(std::string("**OPCODE**"));
}

// ----------------------------------------------------------------------------
void CodeSection::writeCode(FMA::assem::Operand *op) {
  markAsCode();

  if (op->isConstant()) {
    writeCode(std::to_string(op->asConstant()));
    return;
  } else if (op->isResolvedAddress()) {
    writeCode(std::to_string(op->asResolvedAddress()));
    return;
  } else if (op->isSymbolReference()) {
    writeCode(op->asSymbolReference());
    return;
  }

  std::string type(op->getTypeName());
  if (type == "RelativeAddress") {
    FMA::assem::RelativeAddressOperand *sym = dynamic_cast<FMA::assem::RelativeAddressOperand*>(op);
    writeCode(sym->getLeft());
    writeCode("+");
    writeCode(sym->getRight());
  } else {
    std::cerr << "Unsupported operand type: " << type << std::endl;
  }
}

// ----------------------------------------------------------------------------
void CodeSection::writeData(const void *data, uint32_t size) {
  mBlock->write(data, size);
}

// ----------------------------------------------------------------------------
void CodeSection::writeData(const FMA::symbol::ReferencePtr &ref, uint32_t size) {
  mBlock->write(ref, size);
}

// ----------------------------------------------------------------------------
bool CodeSection::generate() const {
  std::map<std::string, FMA::linker::LinkerBlockUserDataPtr> &map(mBlock->getUserData());
  std::map<std::string, FMA::linker::LinkerBlockUserDataPtr>::iterator it;


  it = map.find("FMACPP");
  if (it == map.end()) {
    FMA::linker::LinkerBlockUserDataPtr data(new LinkerBlockCodeSection());
    map["FMACPP"] = data;
    it = map.find("FMACPP");
  }

  std::shared_ptr<LinkerBlockCodeSection> section(std::dynamic_pointer_cast<LinkerBlockCodeSection>(it->second));
  if (!section) {
    return false;
  }

  section->parts.push_back(LinkerBlockCodeSectionPart(mHasCode, mLabel, mCode));
  return true;
}

// ----------------------------------------------------------------------------
