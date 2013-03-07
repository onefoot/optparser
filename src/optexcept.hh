// optp_exc.hh

#ifndef __OPTP_EXC_HH__
#define __OPTP_EXC_HH__

#include <exception>
#include <string>

namespace optparser {

  // class:
  //   optparser_error
  //
  class optparser_error: public std::exception {
    std::string emsg;
    
  public:
    // constructor & destructor
    optparser_error() {}
    explicit optparser_error(const char* s) { if (s) emsg = s; }
    explicit optparser_error(const std::string& str): emsg(str) {}
    ~optparser_error() throw() {}

    // interface
    const char* what() const throw() { return emsg.c_str(); }
  };

}


#endif
