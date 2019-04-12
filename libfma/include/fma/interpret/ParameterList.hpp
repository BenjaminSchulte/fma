#ifndef __FMA_INTERPRET_PARAMETERLIST_H__
#define __FMA_INTERPRET_PARAMETERLIST_H__

#include <vector>
#include <map>
#include "Parameter.hpp"

namespace FMA {
namespace ast{
struct Parameter;
typedef std::shared_ptr<Parameter> ParameterPtr;
}
namespace interpret {
class BaseContext;
typedef std::shared_ptr<BaseContext> ContextPtr;

typedef std::map<std::string, Parameter> ParameterMap;
typedef std::vector<Parameter> ParameterList;
typedef std::map<std::string, types::TypePtr> TypeMap;
typedef std::vector<types::TypePtr> TypeList;

struct GroupedParameterList {

  static GroupedParameterList createFromAstParameters(const ContextPtr &context, const std::vector<ast::ParameterPtr> &parameters);

  inline const TypeList &only_args() const { return args; }
  inline const TypeList &only_blocks() const { return blocks; }
  inline const TypeList &only_context() const { return context; }
  inline const TypeMap &only_kwargs() const { return kwargs; }

  inline TypeList &getArgs() { return args; }

  void push_back(const Parameter &parameter);
  void merge(const GroupedParameterList &other);

  void dump(const ContextPtr &context) const;
  std::string asString(const ContextPtr &context) const;

protected:
  TypeList args;
  TypeList blocks;
  TypeList context;
  TypeMap kwargs;
};

}
}

#endif
