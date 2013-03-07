// ioptdb.hh

#ifndef __IOPTDB_HH__
#define __IOPTDB_HH__

#include <string>
#include "optparserfwd.hh"

namespace optparser {

  // OptDB:
  //   Interface of the option database.
  // 
  class OptDB {
  public:
    virtual ~OptDB() {}
    virtual OptID set_opt_data(char sopt, const char* lopt) = 0;
    virtual OptID find_opt(const std::string& optname) const = 0;
    virtual std::string get_sopt_name(const OptID& oid) const = 0;
    virtual std::string get_lopt_name(const OptID& oid) const = 0;
    virtual size_t get_sopt_length(const OptID& oid) const = 0;
    virtual size_t get_lopt_length(const OptID& oid) const = 0;

    // Factory method
    static OptDB* create_db();
  };
  
}


#endif
