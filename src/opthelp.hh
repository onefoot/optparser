// opthelp.hh

#ifndef __OPTHELP_HH__
#define __OPTHELP_HH__

#include <string>
#include <map>
#include <list>
#include "optdecorator.hh"
#include "optid.hh"

namespace optparser {

  // OptInfo:
  //   An additional infomation for the option.
  //
  class OptInfo {
    std::string lopt_description;
    std::string description;

  public:
    // constructor & destructor
    OptInfo(const std::string& ldesc, const std::string& desc): lopt_description(ldesc), description(desc) {}
    ~OptInfo() {}

    // method
    std::string lopt_desc() const { return lopt_description; }
    std::string desc() const { return description; }
    size_t lopt_desc_len() const { return lopt_description.size(); }
    size_t desc_len() const { return description.size(); }
  };


  // InfoGroup:
  //   A group of OptInfo.
  //
  class InfoGroup {
    std::map<OptID, OptInfo*> oi_units;
    std::string description;

  public:
    // constructor & destructor
    InfoGroup() {}
    explicit InfoGroup(const std::string& desc): description(desc) {}
    ~InfoGroup();

    // method
    std::string get_group_desc() const { return description; }
    size_t get_size() const { return oi_units.size(); }
    bool add_unit(const OptID& oid, const std::string& ldesc, const std::string& desc);
    bool get_optid_list(std::list<OptID>* oidlist) const;
    std::string get_lopt_desc(const OptID& oid) const;
    std::string get_desc(const OptID& oid) const;
    size_t get_lopt_desc_length(const OptID& oid) const;
    size_t get_desc_length(const OptID& oid) const;
  };


  // Constans for OptDBHelp.
  // 
  const size_t c_sopt_col_width = 6;	// size of "  -a, "
  const size_t c_lopt_col_padding = 7;

  
  // OptDBHelp:
  //   The option database with Help message generator.
  //
  class OptDBHelp: public AbstractDecoratorOptDB {
    std::string help_caption;
    std::string help_other_caption;
    size_t width_limit;
    size_t first_col_width; 
    size_t second_col_width;
    size_t third_col_width; 
    std::map<OptID, InfoGroup*> oi_grps;

  public:
    // constructor & destructor
    OptDBHelp(size_t col_size);
    ~OptDBHelp();
    
    // method
    void set_progname(const std::string& pgname);
    void set_caption(const std::string& capstr);
    OptID set_opt_info(const OptID& grpid,
		       char sopt,
		       const char* lopt,
		       const char* ldesc,
		       const char* desc);
    OptID add_group(const char* grpdesc);
    OptID get_help_group_id() const;
    std::string generate_help();
    std::string generate_usage();

  private:
    OptID alloc_grpid();
    void calc_help_col_width();
    void calc_usage_col_width();
    std::string gen_group_help(InfoGroup* grp);
    std::string gen_group_usage(InfoGroup* grp);
  };

}


#endif
