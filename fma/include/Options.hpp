#ifndef __FMA_OPTIONS_H__
#define __FMA_OPTIONS_H__

#include <boost/program_options.hpp>

class Options {
public:
  bool parse(int argc, char **argv);

  inline const boost::program_options::variables_map &args() const {
    return vm;
  };

protected:
  boost::program_options::variables_map vm;
};

#endif
