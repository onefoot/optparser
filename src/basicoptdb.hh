// basicoptdb.hh

#ifndef __BASICOPTDB_HH__
#define __BASICOPTDB_HH__

#include <string>
#include <map>
#include "ioptdb.hh"
#include "optid.hh"

namespace optparser {

  // OptUnit:
  //   A unit for the option data.
  //
  class OptUnit {
    std::string short_name;
    std::string long_name;

  public:
    // constructor & destructor
    OptUnit(const std::string& sopt, const std::string& lopt): short_name(sopt), long_name(lopt) {}
    ~OptUnit() {}
    
    // method
    std::string sname() const { return short_name; }
    std::string lname() const { return long_name; }
    bool check_opt(const std::string& optname);
    size_t sname_len() const { return short_name.size(); }
    size_t lname_len() const { return long_name.size(); }
  };


  // BasicOptDB:
  //   The database for the option data.
  //
  class BasicOptDB: public OptDB {
    std::map<OptID, OptUnit*> o_units;
    OptID* last_allocated_id;

  public:
    // constructor & destructor
    BasicOptDB(): last_allocated_id(0) {}
    ~BasicOptDB();

    // method
    OptID set_opt_data(char sopt, const char* lopt);
    OptID find_opt(const std::string& optname) const;
    std::string get_sopt_name(const OptID& oid) const;
    std::string get_lopt_name(const OptID& oid) const;
    size_t get_sopt_length(const OptID& oid) const;
    size_t get_lopt_length(const OptID& oid) const;
    
  private:
    OptID* alloc_optid();
  };

}


#endif
