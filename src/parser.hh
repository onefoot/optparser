// parser.hh

#ifndef __PARSER_HH__
#define __PARSER_HH__

#include <string>
#include <list>
#include "optparserfwd.hh"
#include "optid.hh"

namespace optparser {

  // Parser:
  //   A parser of the options.
  //
  class Parser {

    // Constants of Parser.
    // 
    enum OPTARG_TYPE {
      OAT_UNKNOWN,
      OAT_SHORT,		// -x
      OAT_LONG,			// --yy
      OAT_LONG_COMBINED,	// --zz=DESC
      OAT_VALUE, 		// Not option.
    };

    enum OPT_CLASS {
      CLS_NORMAL,
      CLS_HELP,
      CLS_USAGE,
    };
    
    // ParseUnit:
    //   A data structure for parsing.
    // 
    struct ParseUnit {
      const char* src;		// A unit of argv.
      OPTARG_TYPE type;
      OPT_CLASS cls;
      OptID oid;
      std::string value;
    };

    OptCommand* optcmd;
    std::list<ParseUnit> parsed_args;
    std::list<std::string> lo_args;

  public:
    // constructor & destructor
    Parser(OptCommand* ocmd): optcmd(ocmd) {}
    ~Parser() {}

    // method
    bool parse(char** src_argv, size_t n_argv);

  private:
    // For parsing.
    void assort_type(char** src_argv, size_t n_argv);
    void assort_opt_val();
    void analyze_opt();
    void check_duplicate();
    OPTARG_TYPE check_arg_type(const char* src_arg);

    // Utility.
    bool divide_combined_arg(const char* src_arg,
			     const char sep,
			     std::string* dst_opt,
			     std::string* dst_val);
  };

}


#endif
