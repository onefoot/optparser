// optvalfact.hh

#ifndef __OPTVALFACT_HH__
#define __OPTVALFACT_HH__

#include <string>
#include "optparserfwd.hh"

namespace optparser {

  // OptValFact:
  //   A factory object for OptVal.
  // 
  class OptValFact {
  public:
    static OptVal* conv(int* dst);
    static OptVal* conv(float* dst);
    static OptVal* conv(double* dst);
    static OptVal* conv(char* dst);
    static OptVal* conv(std::string* dst);
    static OptVal* conv(bool* dst);
  };
  
}


#endif
