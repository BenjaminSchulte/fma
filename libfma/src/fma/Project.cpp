#include <fma/Project.hpp>
#include <fma/FileMap.hpp>
#include <fma/Log.hpp>
#include <fma/types/RootModule.hpp>
#include <fma/serialize/SerializerRegistry.hpp>

#include <boost/locale.hpp>

using namespace FMA;
using namespace FMA::serialize;

// ----------------------------------------------------------------------------
Project::Project()
  : files(new FileMap(this))
  , _log(new Log())
  , _defaultLogAdapter(new ConsoleLog())
  , _serializer(new SerializerRegistry())
  , _root(new types::RootModule())
{
  boost::locale::generator gen;
  std::locale::global(gen("")); 

  _log->setLogAdapter(_defaultLogAdapter);
}

// ----------------------------------------------------------------------------
Project::~Project() {
  _root->unload();

  delete _log;
  delete _defaultLogAdapter;
  delete _serializer;
  delete files;
}

// ----------------------------------------------------------------------------
void Project::addByteCodeGenerator(plugin::BinaryGeneratorPluginAdapter* adapter) {
  binaryAdapters.push_back(adapter);
}

// ----------------------------------------------------------------------------
void Project::removeAllByteCodeGenerators() {
  binaryAdapters.clear();
}

// ----------------------------------------------------------------------------
