// ioptval.hh

#ifndef __IOPTVAL_HH__
#define __IOPTVAL_HH__

#include <string>

namespace optparser {

  // OptVal:
  //   Interface of the option value object.
  // 
  class OptVal {
  public:
    virtual ~OptVal() {}
    virtual bool has_value() = 0;
    virtual void set_value(const std::string& src) = 0;
    // For bool type.
    virtual void set_value(bool flag) = 0;
  };

}


#endif
