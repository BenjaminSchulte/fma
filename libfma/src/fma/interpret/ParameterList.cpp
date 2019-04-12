#include <fma/ast/Expression.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Pointer.hpp>
#include <fma/Project.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/Result.hpp>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
void GroupedParameterList::push_back(const Parameter &parameter) {
  switch (parameter.type) {
    case Parameter::ARG:
      args.push_back(parameter.value);
      break;

    case Parameter::KWARG:
      kwargs[parameter.name] = parameter.value;
      break;

    case Parameter::CONTEXT:
      context.push_back(parameter.value);
      break;

    case Parameter::BLOCK:
      blocks.push_back(parameter.value);
      break;

    default:
      std::cerr << "Invalid parameter type" << std::endl;
      break;
  }
}

// ----------------------------------------------------------------------------
void GroupedParameterList::merge(const GroupedParameterList &other) {
  std::copy(other.args.cbegin(), other.args.cend(), std::back_inserter(args));
  std::copy(other.context.cbegin(), other.context.cend(), std::back_inserter(context));
  std::copy(other.blocks.cbegin(), other.blocks.cend(), std::back_inserter(blocks));

  for (const auto &pair : other.kwargs) {
    kwargs[pair.first] = pair.second;
  }
}

// ----------------------------------------------------------------------------
GroupedParameterList GroupedParameterList::createFromAstParameters(const ContextPtr &context, const std::vector<ast::ParameterPtr> &parameters) {
  GroupedParameterList params;

  for (auto &param : parameters) {
    ResultPtr result = param->execute(context);

    switch (param->getType()) {
      case ast::Parameter::KWARG:
        params.kwargs[param->getKey()] = result->get();
        break;

      case ast::Parameter::BLOCK:
        params.blocks.push_back(result->get());
        break;

      case ast::Parameter::CONTEXT:
        params.context.push_back(result->get());
        break;

      case ast::Parameter::ARG:
        {
          TypePtr type = result->get();
          if (type->isPointer()) {
            params.blocks.push_back(type->asPointer()->getChild());
          } else {
            params.args.push_back(type);
          }
        }
        break;

      case ast::Parameter::ARGS:
        for (auto &item : result->get()->convertToArray(context)) {
          params.args.push_back(item);
        }
        break;

      case ast::Parameter::KWARGS:
        for (auto &item : result->get()->convertToMap(context)) {
          params.kwargs[item.first] = item.second;
        }
        break;
    
      default:
        context->log().error() << "Unknown parameter type";
        break;
    }
  }

  return params;
}

// ----------------------------------------------------------------------------
std::string GroupedParameterList::asString(const ContextPtr &) const {
  /*std::ostringstream os;
  for (auto &item : *this) {
    switch (item.type) {
      case Parameter::ARG:
        os << item.value->convertToString(context) << ", ";
        break;

      case Parameter::KWARG:
        os << item.name << ": " << item.value->convertToString(context) << ", ";
        break;

      case Parameter::BLOCK:
        os << "&" << item.value->asString() << ", ";
        break;

      default:
        os << "???, " << std::endl;
        break;
    }
  }
  return os.str();*/
  return "<GroupedParameterList>";
}

// ----------------------------------------------------------------------------
void GroupedParameterList::dump(const ContextPtr &) const {
  /*for (auto &item : *this) {
    switch (item.type) {
      case Parameter::ARG:
        std::cout << "- #: " << item.value->convertToString(context) << std::endl;
        break;

      case Parameter::KWARG:
        std::cout << "- " << item.name << ": " << item.value->convertToString(context) << std::endl;
        break;

      case Parameter::BLOCK:
        std::cout << "- &: " << item.value->asString() << std::endl;
        break;

      default:
        std::cout << "- ???" << std::endl;
        break;
    }
  }*/

  std::cout << "TODO: dump" << std::endl;
}

