// optval.hh

#ifndef __OPTVAL_HH__
#define __OPTVAL_HH__

#include <cstdlib>
#include "ioptval.hh"

namespace optparser {

  // ValueInt:
  //   An option value object for Int.
  // 
  class ValueInt: public OptVal {
    int* val;

  public:
    // constructor & destructor
    ValueInt(int* dst): val(dst) {}
    ~ValueInt() {}

    // method
    bool has_value() { return true; }
    void set_value(const std::string& src) { *val = atoi(src.c_str()); }
    void set_value(bool flag) { return; }
  };


  // ValueFloat:
  //   An option value object for Float.
  // 
  class ValueFloat: public OptVal {
    float* val;

  public:
    // constructor & destructor
    ValueFloat(float* dst): val(dst) {}
    ~ValueFloat() {}
    
    // method
    bool has_value() { return true; }
    void set_value(const std::string& src) { *val = static_cast<float>(atof(src.c_str())); }
    void set_value(bool flag) { return; }
  };


  // ValueDouble:
  //   An option value object for Double.
  // 
  class ValueDouble: public OptVal {
    double* val;

  public:
    // constructor & destructor
    ValueDouble(double* dst): val(dst) {}
    ~ValueDouble() {}
    
    // method
    bool has_value() { return true; }
    void set_value(const std::string& src) { *val = atof(src.c_str()); }
    void set_value(bool flag) { return; }
  };


  // ValueString:
  //   An option value object for String.
  // 
  class ValueString: public OptVal {
    std::string* val;

  public:
    // constructor & destructor
    ValueString(std::string* dst): val(dst) {}
    ~ValueString() {}

    // method
    bool has_value() { return true; }
    void set_value(const std::string& src) { *val = src; }
    void set_value(bool flag) { return; }
  };


  // ValueChar:
  //   An option value object for Char.
  // 
  class ValueChar: public OptVal {
    char* val;

  public:
    // constructor & destructor
    ValueChar(char* dst): val(dst) {}
    ~ValueChar() {}
    
    // method
    bool has_value() { return true; }
    void set_value(const std::string& src) { *val = src[0]; }
    void set_value(bool flag) { return; }
  };


  // ValueBool:
  //   An option value object for Bool.
  // 
  class ValueBool: public OptVal {
    bool* val;

  public:
    // constructor & destructor
    ValueBool(bool* dst): val(dst) {}
    ~ValueBool() {}

    // method
    bool has_value() { return false; }
    void set_value(const std::string& src) { return; }
    void set_value(bool flag) { *val = flag; }
  };

}


#endif
