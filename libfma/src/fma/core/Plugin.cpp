#include <fma/core/Plugin.hpp>
#include <fma/core/Object.hpp>
#include <fma/core/Class.hpp>
#include <fma/core/DataBlock.hpp>
#include <fma/core/Function.hpp>
#include <fma/core/Compiler.hpp>
#include <fma/core/String.hpp>
#include <fma/core/Number.hpp>
#include <fma/core/Range.hpp>
#include <fma/core/Array.hpp>
#include <fma/core/Buffer.hpp>
#include <fma/core/SerializerBuffer.hpp>
#include <fma/core/DeserializerBuffer.hpp>
#include <fma/core/Map.hpp>
#include <fma/core/Nil.hpp>
#include <fma/core/Boolean.hpp>
#include <fma/core/SymbolReference.hpp>
#include <fma/core/ImmediateNumber.hpp>
#include <fma/core/TypedNumber.hpp>
#include <fma/serialize/SerializerRegistry.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/Project.hpp>

using namespace FMA;
using namespace FMA::core;
using namespace FMA::types;

// ----------------------------------------------------------------------------
CorePlugin::CorePlugin(Project *project)
  : LanguagePlugin(project)
{
}

// ----------------------------------------------------------------------------
bool CorePlugin::initialize() {
  const RootModulePtr &root = project->root();

  ClassPtr Object = ObjectClass::create(root);
  ClassPtr Klass = ClassClass::create(root, Object);

  NumberClass::create(root, Klass);
  ImmediateNumberClass::create(root, Klass);
  
  CompilerClass::create(root, Klass);
  StringClass::create(root, Klass);
  RangeClass::create(root, Klass);
  BooleanClass::create(root, Klass);
  ArrayClass::create(root, Klass);
  BufferClass::create(root, Klass);
  MapClass::create(root, Klass);
  NilClass::create(root, Klass);
  SymbolReferenceClass::create(root, Klass);
  TypedNumberClass::create(root, Klass);
  SerializerBufferClass::create(root, Klass);
  DeserializerBufferClass::create(root, Klass);

  project->serializer()->registerSerializer(serialize::TYPE_NIL, NilClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_ARRAY, ArrayClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_BOOLEAN, BooleanClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_BUFFER, BufferClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_MAP, MapClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_NUMBER, NumberClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_OBJECT, ObjectClass::serializeObject);
  project->serializer()->registerSerializer(serialize::TYPE_STRING, StringClass::serializeObject);
  
  project->serializer()->registerDeserializer(serialize::TYPE_NIL, NilClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_ARRAY, ArrayClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_BOOLEAN, BooleanClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_BUFFER, BufferClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_MAP, MapClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_NUMBER, NumberClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_OBJECT, ObjectClass::deserializeObject);
  project->serializer()->registerDeserializer(serialize::TYPE_STRING, StringClass::deserializeObject);

  ClassPtr DataBlock = DataBlockClass::create(root, Klass);
  FunctionClass::create(root, DataBlock);

  return true;
}

// ----------------------------------------------------------------------------
void CorePlugin::release() {

}

// ----------------------------------------------------------------------------