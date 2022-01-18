#pragma once

#include <string>
#include <sstream>
#include <fma/symbol/Reference.hpp>
#include <fma/assem/Operand.hpp>
#include <fma/linker/LinkerBlock.hpp>

namespace FMACPP {
namespace binary {

struct LinkerBlockCodeSectionCode {
  enum Type {
    CODE,
    SYMBOL
  };

  LinkerBlockCodeSectionCode() {}
  LinkerBlockCodeSectionCode(const std::string &code) : type(CODE), code(code) {}
  LinkerBlockCodeSectionCode(const FMA::symbol::SymbolReferencePtr &ref) : type(SYMBOL), ref(ref) {}

  Type type;
  std::string code;
  FMA::symbol::SymbolReferencePtr ref;
};

struct LinkerBlockCodeSectionPart {
  LinkerBlockCodeSectionPart() {}
  LinkerBlockCodeSectionPart(bool has, const std::string &label, const std::vector<LinkerBlockCodeSectionCode> &code) : hasCode(has), label(label), code(code) {}

  bool hasCode = false;
  std::string label;
  std::vector<LinkerBlockCodeSectionCode> code;
};

struct LinkerBlockCodeSection : public FMA::linker::LinkerBlockUserData {
  static std::string TypeId() { return "FMACPP::binary::LinkerBlockCodeSection"; }
  std::string typeId() const override { return TypeId(); }

  std::vector<LinkerBlockCodeSectionPart> parts;
};

class CodeSection {
public:
  //! Constructor
  CodeSection(int id, const std::string &label, FMA::linker::LinkerBlock *block);

  //! Returns the ID
  inline int id() const { return mId; }

  //! Returns the label
  inline const std::string &label() const { return mLabel; }

  //! Writes CPP code
  void writeCode(const std::string &);

  //! Writes CPP code
  void writeCode(FMA::assem::Operand *);

  //! Writes CPP code
  void writeCode(const FMA::symbol::SymbolReferencePtr &);

  //! Writes CPP code
  void writeCode(const FMA::symbol::ReferencePtr &);

  //! Writes data
  void writeData(const void*, uint32_t);

  //! Writes data
  void writeData(const FMA::symbol::ReferencePtr &, uint32_t);

  //! Generates the final code
  bool generate() const;

  //! Marks this section as code
  void markAsCode();

  //! Returns whether this is a code section
  inline bool isCodeSection() const { return mHasCode; }

private:
  //! The ID of the section
  int mId;

  //! The label
  std::string mLabel;

  //! The linker block
  FMA::linker::LinkerBlock *mBlock;

  //! The code
  std::vector<LinkerBlockCodeSectionCode> mCode;

  //! Whether any code exists
  bool mHasCode = false;
};

}
}