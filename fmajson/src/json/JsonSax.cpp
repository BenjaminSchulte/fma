#include <json/JsonSax.hpp>

#include <fma/interpret/Result.hpp>
#include <fma/core/Nil.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/String.hpp>

#include <iostream>

using namespace FMA::json;
using namespace FMA::core;
using namespace FMA::interpret;
using namespace FMA::types;

// ----------------------------------------------------------------------------
JsonSax::JsonSax(const ContextPtr &context)
  : currentKey("")
  , context(context)
{
}

// ----------------------------------------------------------------------------
void JsonSax::setValue(const TypePtr &type) {
  if (stack.size()) {
    if (stack.back().isArray) {
      stack.back().array.push_back(type);
    } else {
      stack.back().map.insert(std::make_pair(currentKey, type));
    }
  } else {
    root = type;
  }
}

// ----------------------------------------------------------------------------
bool JsonSax::key(string_t& val) {
  currentKey = val;
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::null() {
  setValue(NilClass::createInstance(context)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::boolean(bool val) {
  setValue(BooleanClass::createInstance(context, val)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::number_integer(number_integer_t val) {
  setValue(NumberClass::createInstance(context, val)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::number_unsigned(number_unsigned_t val) {
  setValue(NumberClass::createInstance(context, val)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::number_float(number_float_t val, const string_t&) {
  setValue(NumberClass::createInstance(context, val)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::string(string_t& val) {
  setValue(StringClass::createInstance(context, val)->get());
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::start_object(std::size_t) {
  stack.push_back(JsonSaxStackItem(currentKey, false));
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::end_object() {
  if (!stack.size()) {
    return false;
  }

  const JsonSaxStackItem &back = stack.back();
  if (back.isArray) {
    return false;
  }

  TypePtr map = MapClass::createInstance(context, back.map)->get();
  
  currentKey = back.key;
  stack.pop_back();

  setValue(map);
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::start_array(std::size_t) {
  stack.push_back(JsonSaxStackItem(currentKey, true));
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::end_array() {
  if (!stack.size()) {
    return false;
  }

  const JsonSaxStackItem &back = stack.back();
  if (!back.isArray) {
    return false;
  }

  TypePtr array = ArrayClass::createInstance(context, back.array)->get();

  currentKey = back.key;
  stack.pop_back();

  setValue(array);
  return true;
}

// ----------------------------------------------------------------------------
bool JsonSax::parse_error(std::size_t position, const std::string& last_token, const nlohmann::detail::exception& ex) {
  context->log().error() << "At position " << position << ": " << ex.what() << " (last token: " << last_token << ")";
  return false;
}