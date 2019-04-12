#include <sstream>
#include <fma/ast/Expression.hpp>
#include <fma/types/Base.hpp>
#include <fma/types/Class.hpp>
#include <fma/types/Object.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/interpret/BaseContext.hpp>
#include <fma/interpret/ObjectContext.hpp>
#include <fma/interpret/Result.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::ast;
using namespace FMA::core;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
OperationExpression::OperationExpression(Expression *left, OperationExpression::Operator op, Expression *right)
  : left(left)
  , op(op)
  , right(right)
{
}

// ----------------------------------------------------------------------------
std::string OperationExpression::asString(const std::string &) const {
  std::ostringstream os;
  os << left->asString("");
  switch (op) {
    case ADD: os << "+"; break;
    case MUL: os << "*"; break;
    case DIV: os << "/"; break;
    case SUB: os << "-"; break;
    case AND: os << "&"; break;
    case REM: os << "%"; break;
    case OR: os << "|"; break;
    case XOR: os << "^"; break;
    case LSHIFT: os << "<<"; break;
    case RSHIFT: os << ">>"; break;
    case LOG_AND: os << "&&"; break;
    case LOG_OR: os << "||"; break;
    case EQUALITY: os << "=="; break;
    case NOT_EQUALITY: os << "!="; break;
    case LESSTHAN_EQUAL: os << "<="; break;
    case GREATERTHAN_EQUAL: os << ">="; break;
    case LESSTHAN: os << "<"; break;
    case GREATERTHAN: os << ">"; break;
    case MEMBER: os << "."; break;
    case INDEX: os << "["; break;
    case ASSIGN_LOG_AND: os << "&&="; break;
    case ASSIGN_LOG_OR: os << "||="; break;
    case ASSIGN_ADD: os << "+="; break;
    case ASSIGN_SUB: os << "-="; break;
    case ASSIGN_MUL: os << "*="; break;
    case ASSIGN_DIV: os << "/="; break;
    case ASSIGN_REM: os << "%="; break;
    case ASSIGN_AND: os << "&="; break;
    case ASSIGN_OR: os << "|="; break;
    case ASSIGN_XOR: os << "^="; break;
    case ASSIGN_LSHIFT: os << "<<="; break;
    case ASSIGN_RSHIFT: os << ">>="; break;

    default: os << "???"; break;
  }
  os << right->asString("");
  switch (op) {
    case INDEX: os << "]"; break;

    default: break;
  }
  return os.str();
}

// ---------------------------------------------------------------------------
ResultPtr OperationExpression::calculate(const ContextPtr &context, const ResultPtr &left, const ResultPtr &right, const char *op) const {
  TypePtr lside = left->get();

  if (!lside->hasMember(op)) {
    context->log().error() << location << "Unable to execute operator " << op << " on " << left->asString();
    return ResultPtr(new Result());
  }

  ContextPtr objContext(new ObjectContext(context->getInterpreter(), lside));
  interpret::GroupedParameterList parameters;
  parameters.push_back(interpret::Parameter(right->get()));
  return lside->getMember(op)->call(objContext, parameters);
}

// ---------------------------------------------------------------------------
bool OperationExpression::isAssign(Operator op) {
  switch (op) {
  case ASSIGN_LOG_AND:
  case ASSIGN_LOG_OR:
  case ASSIGN_ADD:
  case ASSIGN_SUB:
  case ASSIGN_MUL:
  case ASSIGN_DIV:
  case ASSIGN_REM:
  case ASSIGN_AND:
  case ASSIGN_OR:
  case ASSIGN_XOR:
  case ASSIGN_LSHIFT:
  case ASSIGN_RSHIFT:
    return true;

  default:
    return false;
  }
}

// ---------------------------------------------------------------------------
ResultPtr OperationExpression::execute(const ContextPtr &context) const {
  ResultPtr leftResult = context->execute(left);
  ResultPtr result;

  if (op != LOG_OR && op != ASSIGN_LOG_OR && leftResult->isUndefined()) {
    context->log().error() << location << "Expression is undefined";
    return ResultPtr(new Result());
  }

  switch (op) {
  case ASSIGN_ADD: case ADD: result = calculate(context, leftResult, context->execute(right), "+"); break;
  case ASSIGN_SUB: case SUB: result = calculate(context, leftResult, context->execute(right), "-"); break;
  case ASSIGN_MUL: case MUL: result = calculate(context, leftResult, context->execute(right), "*"); break;
  case ASSIGN_DIV: case DIV: result = calculate(context, leftResult, context->execute(right), "/"); break;
  case ASSIGN_REM: case REM: result = calculate(context, leftResult, context->execute(right), "%"); break;
  case ASSIGN_AND: case AND: result = calculate(context, leftResult, context->execute(right), "&"); break;
  case ASSIGN_OR: case OR: result = calculate(context, leftResult, context->execute(right), "|"); break;
  case ASSIGN_XOR: case XOR: result = calculate(context, leftResult, context->execute(right), "^"); break;
  case ASSIGN_LSHIFT: case LSHIFT: result = calculate(context, leftResult, context->execute(right), "<<"); break;
  case ASSIGN_RSHIFT: case RSHIFT: result = calculate(context, leftResult, context->execute(right), ">>"); break;

  case LESSTHAN_EQUAL:
    result = calculate(context, leftResult, context->execute(right), "==");
    if (result->get()->convertToBoolean(context)) {
      break;
    }
    // !!! NO BREAK !!!
  case LESSTHAN:
    if (leftResult->get()->hasMember("<")) {
      result = calculate(context, leftResult, context->execute(right), "<");
    } else {
      result = calculate(context, leftResult, context->execute(right), ">");
      if (result->get()->convertToBoolean(context)) {
        result = BooleanClass::createInstance(context, false);
      } else {
        result = calculate(context, leftResult, context->execute(right), "==");
        result = BooleanClass::createInstance(context, !result->get()->convertToBoolean(context));
      }
    }
    break;

  case GREATERTHAN_EQUAL:
    result = calculate(context, leftResult, context->execute(right), "==");
    if (result->get()->convertToBoolean(context)) {
      break;
    }
    // !!! NO BREAK !!!
  case GREATERTHAN:
    if (leftResult->get()->hasMember(">")) {
      result = calculate(context, leftResult, context->execute(right), ">");
    } else {
      result = calculate(context, leftResult, context->execute(right), "<");
      if (result->get()->convertToBoolean(context)) {
        result = BooleanClass::createInstance(context, false);
      } else {
        result = calculate(context, leftResult, context->execute(right), "==");
        result = BooleanClass::createInstance(context, !result->get()->convertToBoolean(context));
      }
    }
    break;

  case EQUALITY: result = calculate(context, leftResult, context->execute(right), "=="); break;
  case NOT_EQUALITY:
    result = calculate(context, leftResult, context->execute(right), "==");
    result = BooleanClass::createInstance(context, !result->get()->convertToBoolean(context));
    break;

  case ASSIGN_LOG_OR:
  case LOG_OR:
    if (!leftResult->isUndefined() && leftResult->get()->convertToBoolean(context)) {
      result = leftResult;
    } else {
      result = context->execute(right);
    }
    break;

  case ASSIGN_LOG_AND:
  case LOG_AND:
    if (leftResult->get()->convertToBoolean(context)) {
      result = context->execute(right);
    } else {
      result = BooleanClass::createInstance(context, false);
    }
    break;

  case INDEX:
    if (leftResult->get()->hasMember("[]")) {
      result = calculate(context, leftResult, context->execute(right), "[]");
      break;
    }
    // no break !!
  case MEMBER:
    {
      TypePtr owner = leftResult->get();

      std::string name = context->execute(right)->get()->convertToString(context);

      result = ResultPtr(new Result(context, owner->getDirectMember(name), owner));
      result->makeAssignable(ContextPtr(new ObjectContext(context->getInterpreter(), owner)), name);

      if (result->isUndefined() && leftResult->get()->hasMember(".")) {
        interpret::GroupedParameterList paramRight;
        paramRight.push_back(context->execute(right)->get());
        ResultPtr otherResult = leftResult->get()->callDirect(".", context, paramRight);
        
        if (!otherResult->isUndefined()) {
          result = otherResult;
        }
      }
    }
    break;

  default:
    std::cout << "TODO: Unhandled operator in OperationExpression "<< op << std::endl;
    result = leftResult;
    break;
  }

  if (isAssign(op)) {
    if (leftResult->isAssignable()) {
      leftResult->assign(result->get());
    } else {
      context->log().error() << location << "Could not assign to left side variable";
      result = context->execute(right);
    }
  }

  return result;
}


// ----------------------------------------------------------------------------
