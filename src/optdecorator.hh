// optdecorator.hh

#ifndef __OPTDECORATOR_HH__
#define __OPTDECORATOR_HH__

#include "ioptdb.hh"
#include "optid.hh"

namespace optparser {

  // AbstractDecoratorOptDB:
  //   The abstract decorator for OptDB.
  // 
  class AbstractDecoratorOptDB: public OptDB {
    OptDB* odb;

  public:
    // constructor & destructor
    AbstractDecoratorOptDB(OptDB* db): odb(db) {}
    ~AbstractDecoratorOptDB() { delete odb; }
    
    // method
    OptID set_opt_data(char sopt, const char* lopt) { return odb->set_opt_data(sopt, lopt); }
    OptID find_opt(const std::string& optname) const { return odb->find_opt(optname); }
    std::string get_sopt_name(const OptID& oid) const { return odb->get_sopt_name(oid); }
    std::string get_lopt_name(const OptID& oid) const { return odb->get_lopt_name(oid); }
    size_t get_sopt_length(const OptID& oid) const { return odb->get_sopt_length(oid); }
    size_t get_lopt_length(const OptID& oid) const { return odb->get_lopt_length(oid); }
  };

}


#endif
