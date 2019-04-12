#include <fma/ast/Expression.hpp>
#include <fma/ast/Identifier.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Map.hpp>
#include <fma/types/Block.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Pointer.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::core;

// ----------------------------------------------------------------------------
Block::Block()
{
}

// ----------------------------------------------------------------------------
void Block::setMembersFromParameter(const ContextPtr &callContext, const ContextPtr &blockContext, const ast::MacroParameterList &expected, const GroupedParameterList &given) {
  ast::MacroParameterList::const_iterator currentExpected = expected.begin();
  const TypeList &args = given.only_args();
  const TypeList &blocks = given.only_blocks();
  TypeMap kwargs = given.only_kwargs();
  TypeList::const_iterator currentArgs = args.begin();

  std::string argsName;
  std::string kwargsName;
  std::string blockName = "";

  while (currentExpected != expected.end()) {
    std::string identifier = currentExpected->get()->getIdentifier()->asString("");
    TypeMap::const_iterator kwarg = kwargs.find(identifier);

    if (currentExpected->get()->isArgs()) {
      argsName = identifier;
    } else if (currentExpected->get()->isKwArgs()) {
      kwargsName = identifier;
    } else if (currentExpected->get()->isBlock()) {
      blockName = identifier;
    } else if (kwarg != kwargs.end()) {
      setMember(identifier, kwarg->second);
      kwargs.erase(kwarg);
    } else if (currentArgs != args.end()) {
      setMember(identifier, *currentArgs);
      currentArgs++;
    } else if (currentExpected->get()->hasDefaultValue()) {
      setMember(identifier, currentExpected->get()->getDefaultValue()->execute(blockContext)->get());
    } else {
      callContext->log().warn() << "Missing argument in call: " << identifier;
    }

    currentExpected++;
  }

  if (blocks.size()) {
    if (blockName == "") {
      setMember("yield", blocks.front());
    } else {
      setMember(blockName, PointerPtr(new Pointer(blocks.front())));
    }
  }

  if (argsName.length()) {
    GroupedParameterList unusedArgs;
    std::copy(currentArgs, args.cend(), std::back_inserter(unusedArgs.getArgs()));
    setMember(argsName, ArrayClass::createInstance(blockContext, unusedArgs.only_args())->get());
  }

  if (kwargsName.length()) {
    std::map<std::string, TypePtr> map;
    for (auto &item : kwargs) {
      map[item.first] = item.second;
    }
    setMember(kwargsName, MapClass::createInstance(blockContext, map)->get());
  }
}

// ----------------------------------------------------------------------------
ResultPtr Block::callWithoutDecoratorTest(const ContextPtr &context, const GroupedParameterList&) {
  return Result::executed(context, getPointer());
}

// ----------------------------------------------------------------------------
std::string Block::asString() const {
  std::ostringstream os;
  os << "<Block @ " << std::hex << this << ">";
  return os.str();
}

// ----------------------------------------------------------------------------

