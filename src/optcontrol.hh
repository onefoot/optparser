// optcontrol.hh

#ifndef __OPTCONTROL_HH__
#define __OPTCONTROL_HH__

#include <string>
#include <list>
#include <map>
#include "optparserfwd.hh"
#include "ioptcmd.hh"
#include "optid.hh"

namespace optparser {

  // OptControl:
  //   The controller of the options.
  // 
  class OptControl: public OptCommand {
    OptDBHelp* opt_db;
    std::map<OptID, OptVal*> opt_values;
    std::pair<OptID, bool> help_config;
    std::pair<OptID, bool> usage_config;
    std::list<std::string> leftovers;

  public:
    // constructor & destructor
    OptControl();
    ~OptControl();

    // method
    OptID get_optid(const std::string& optname);
    bool check_has_value(const OptID& oid);
    bool check_help_opt(const OptID& oid);
    bool check_usage_opt(const OptID& oid);
    void set_help_enable();
    void set_usage_enable();
    void set_leftover_args(const std::list<std::string>& largs);
    void set_parsed_values(const std::map<OptID, std::string>& pvals);

    OptID add_group(const char* desc);
    void set_option(OptID grpid,
		    OptVal* pval,
		    char sopt,
		    const char* lopt,
		    const char* ldesc,
		    const char* desc);
    void set_help_option(char sopt,
			 const char* lopt,
			 const char* desc);
    void set_usage_option(char sopt,
			  const char* lopt,
			  const char* desc);
    void set_help_caption(const char* msg);
    bool parse(int o_argc, char** o_argv);
    std::list<std::string> get_leftover_args();
    
  private:
    void set_progname_to_db(const char* pgname);
    void show_help();
    void show_usage();

    // This object doesn't allow copying.
    OptControl(const OptControl& src);
    const OptControl& operator=(const OptControl& src);
  };

}


#endif
