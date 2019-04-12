#ifndef __FMAJS_PROJECT_H__
#define __FMAJS_PROJECT_H__ 

#include <nan.h>
#include <fma/plugin/Plugin.hpp>

namespace FMA {
  class Project;
}

typedef std::shared_ptr<FMA::plugin::Plugin> PluginPtr;
typedef std::vector<PluginPtr> PluginList;
typedef std::shared_ptr<FMA::plugin::OutputFileWriterPlugin> OutputFileWriterPluginPtr;

class Project : public Nan::ObjectWrap {
public:
  static Nan::MaybeLocal<v8::Function> CREATE();

  inline FMA::Project *getProject() const { return project; }

protected:
  explicit Project();
  ~Project();

  static NAN_METHOD(AddIncludeDirectory);

  static NAN_METHOD(New);

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  FMA::Project *project;
  PluginList plugins;
  std::vector<OutputFileWriterPluginPtr> outputFileWriters;

  bool loadPlugins();
  bool loadLanguagePlugins(const std::string &language);
  bool loadMemoryPlugins();
  bool loadPlugin(const PluginPtr &plugin);
  bool loadPlugins(const PluginList &);
  void unloadPlugins();
};

#endif
