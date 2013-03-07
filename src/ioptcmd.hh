// ioptcmd.hh

#ifndef __IOPTCMD_HH__
#define __IOPTCMD_HH__

#include <string>
#include <list>
#include <map>
#include "optparserfwd.hh"

namespace optparser {

  // OptCommand:
  //   Interface of the option control object.
  // 
  class OptCommand {
  public:
    virtual ~OptCommand() {}
    virtual OptID get_optid(const std::string& optname) = 0;
    virtual bool check_has_value(const OptID& oid) = 0;
    virtual bool check_help_opt(const OptID& oid) = 0;
    virtual bool check_usage_opt(const OptID& oid) = 0;
    virtual void set_help_enable() = 0;
    virtual void set_usage_enable() = 0;
    virtual void set_leftover_args(const std::list<std::string>& largs) = 0;
    virtual void set_parsed_values(const std::map<OptID, std::string>& pvals) = 0;
  };

}


#endif
