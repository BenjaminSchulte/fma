#include <csv/CsvClass.hpp>
#include <aria/csv.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Array.hpp>
#include <fma/types/ClassPrototype.hpp>
#include <fma/types/Undefined.hpp>
#include <fma/types/Object.hpp>
#include <fma/interpret/ParameterList.hpp>
#include <fma/Project.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace FMA;
using namespace FMA::types;
using namespace FMA::interpret;
using namespace FMA::csv;
using namespace FMA::core;

// ----------------------------------------------------------------------------
ClassPtr CsvClass::create(const ModulePtr &root, const ClassPtr &ClassObject) {
  ClassPtr klass = ClassPtr(new Class("CSV", "fmaCSV::CSV"));
  klass->extends(ClassObject);

  klass->setMember("read", TypePtr(new InternalFunctionValue("read", CsvClass::read)));
  klass->setMember("foreach", TypePtr(new InternalFunctionValue("foreach", CsvClass::foreach)));

  ClassPrototypePtr proto(klass->getPrototype());
  proto->setMember("initialize", TypePtr(new InternalFunctionValue("initialize", CsvClass::initialize)));

  root->setMember("CSV", klass);
  return klass;
}

// ----------------------------------------------------------------------------
ResultPtr CsvClass::initialize(const ContextPtr &context, const GroupedParameterList &) {
  context->log().error() << "Can not instanciate class 'CSV'";

  return ResultPtr(new Result());
}

// ----------------------------------------------------------------------------
ResultPtr CsvClass::read(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;
  if (!args.size()) {
    context->log().error() << "Missing filename";
    return ResultPtr(new Result());
  }

  char delimiter = ',';
  char quote = '\"';
  uint32_t headerRows = 0;

  if ((it = kwargs.find("delimiter")) != kwargs.end()) {
    delimiter = it->second->convertToString(context)[0];
  }

  if ((it = kwargs.find("quote")) != kwargs.end()) {
    quote = it->second->convertToString(context)[0];
  }

  if ((it = kwargs.find("num_header_rows")) != kwargs.end()) {
    headerRows = it->second->convertToNumber(context);
  }

  std::ifstream f(args.front()->convertToString(context));
  aria::csv::CsvParser parser(f);
  parser.delimiter(delimiter).quote(quote);

  TypeList rows;
  for (auto &row : parser) {
    TypeList result;
    for (auto &field : row) {
      result.push_back(StringClass::createInstance(context, field)->get());
    }

    if (headerRows) {
      headerRows--;
    } else {
      rows.push_back(ArrayClass::createInstance(context, result)->get());
    }
  }

  return ArrayClass::createInstance(context, rows);
}

// ----------------------------------------------------------------------------
ResultPtr CsvClass::foreach(const ContextPtr &context, const GroupedParameterList &parameter) {
  const TypeList &args = parameter.only_args();
  const TypeList &blocks = parameter.only_blocks();
  const TypeMap &kwargs = parameter.only_kwargs();
  TypeMap::const_iterator it;
  if (!args.size() || !blocks.size()) {
    context->log().error() << "Missing filename or block";
    return ResultPtr(new Result());
  }

  char delimiter = ',';
  char quote = '\"';
  uint32_t headerRows = 0;

  if ((it = kwargs.find("delimiter")) != kwargs.end()) {
    delimiter = it->second->convertToString(context)[0];
  }

  if ((it = kwargs.find("quote")) != kwargs.end()) {
    quote = it->second->convertToString(context)[0];
  }

  if ((it = kwargs.find("num_header_rows")) != kwargs.end()) {
    headerRows = it->second->convertToNumber(context);
  }

  std::ifstream f(args.front()->convertToString(context));
  aria::csv::CsvParser parser(f);
  parser.delimiter(delimiter).quote(quote);

  for (auto &row : parser) {
    TypeList result;
    for (auto &field : row) {
      result.push_back(StringClass::createInstance(context, field)->get());
    }

    if (headerRows) {
      headerRows--;
    } else {
      GroupedParameterList rowParams;
      rowParams.push_back(ArrayClass::createInstance(context, result)->get());
      blocks.front()->call(context, rowParams);
    }
  }

  return ResultPtr(new Result());
}
