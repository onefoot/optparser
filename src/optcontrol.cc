// optcontrol.cc

#include <algorithm>
#include <iostream>
#include "optcontrol.hh"
#include "opthelp.hh"
#include "ioptval.hh"
#include "optvalfact.hh"
#include "parser.hh"
#include "optexcept.hh"

using namespace optparser;
using std::string;
using std::map;
using std::make_pair;
using std::not_equal_to;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


// OptControl:
//   Member functions.
//
OptControl::OptControl(): opt_db(0)
{
  try {
    opt_db = new OptDBHelp();
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'OptDBHelp'.");
  }
}


OptControl::~OptControl()
{
  for (map<OptID, OptVal*>::const_iterator i = opt_values.begin(); i != opt_values.end(); ++i) {
    delete i->second;
  }
  delete opt_db;
}


OptID
OptControl::get_optid(const std::string& optname)
{
  return opt_db->find_opt(optname);
}


bool
OptControl::check_has_value(const OptID& oid)
{
  map<OptID, OptVal*>::const_iterator i = opt_values.find(oid);
  if (i == opt_values.end()) {
    throw optparser_error("OptionID is not found.");
  }
  return i->second->has_value();
}


bool
OptControl::check_help_opt(const OptID& oid)
{
  return help_config.first == oid;
}


bool
OptControl::check_usage_opt(const OptID& oid)
{
  return usage_config.first == oid;
}


void
OptControl::set_help_enable()
{
  help_config.second = true;
}


void
OptControl::set_usage_enable()
{
  usage_config.second = true;
}


void
OptControl::set_leftover_args(const std::list<std::string>& largs)
{
  leftovers = largs;
}


void
OptControl::set_parsed_values(const std::map<OptID, std::string>& pvals)
{
  for (map<OptID, string>::const_iterator i = pvals.begin(); i != pvals.end(); ++i) {
    map<OptID, OptVal*>::iterator p = opt_values.find(i->first);
    if (p == opt_values.end()) {
      throw optparser_error("OptionID don't match.");
    }

    if (i->second.empty()) {
      p->second->set_value(true);
    }
    else {
      p->second->set_value(i->second);
    }
  }
}


OptID
OptControl::add_group(const char* desc)
{
  return opt_db->add_group(desc);
}


void
OptControl::set_option(OptID grpid,
		       OptVal* pval,
		       char sopt,
		       const char* lopt,
		       const char* ldesc,
		       const char* desc)
{
  if (! pval) {
    throw optparser_error("OptVal object is null.");
  }
  OptID new_oid = opt_db->set_opt_info(grpid, sopt, lopt, ldesc, desc);
  opt_values.insert(make_pair(new_oid, pval));
}


void
OptControl::set_help_option(char sopt,
			    const char* lopt,
			    const char* desc)
{
  if (help_config.first) {
    throw optparser_error("Duplicate the configuration of the help option.");
  }
  OptID help_gid = opt_db->get_help_group_id();
  help_config.first = opt_db->set_opt_info(help_gid, sopt, lopt, 0, desc);
  help_config.second = false;
}


void
OptControl::set_usage_option(char sopt,
			     const char* lopt,
			     const char* desc)
{
  if (usage_config.first) {
    throw optparser_error("Duplicate the configuration of the usage option.");
  }
  OptID help_gid = opt_db->get_help_group_id();
  usage_config.first = opt_db->set_opt_info(help_gid, sopt, lopt, 0, desc);
  usage_config.second = false;
}


void
OptControl::set_help_caption(const char* msg)
{
  if (! msg) {
    return;
  }
  string tmp(msg);
  opt_db->set_caption(tmp);
}


bool
OptControl::parse(int o_argc, char** o_argv)
{
  if (o_argc < 1) {
    throw optparser_error("Invalid parameters, 'o_argc'");
  }
  for (int i = 0; i != o_argc; ++i) {
    if (! o_argv || ! o_argv[i]) {
      throw optparser_error("Invalid parameters, 'o_argv'");
    }
  }

  if (! o_argc || ! o_argv) {
    throw optparser_error("Invalid parameters, 'o_argc'");
  }

  // Setting the program name.
  set_progname_to_db(o_argv[0]);
  
  // Parsing arguments.
  Parser* psr = new Parser(this);
  bool applied_val = psr->parse(o_argv + 1, o_argc - 1);

  if (! applied_val) {
    // Printing Help and Usage.
    if (help_config.second) {
      show_help();
      goto done;
    }
    if (usage_config.second) {
      show_usage();
      goto done;
    }
  }

 done:
  delete psr;
  return applied_val;
}


std::list<std::string>
OptControl::get_leftover_args()
{
  return leftovers;
}


// OptControl:
//   Private member functions.
//
void
OptControl::set_progname_to_db(const char* pgname)
{
  if (! pgname) {
    return;
  }
  
  string progname;
  string tmp_progname(pgname);
  string::reverse_iterator delim = find(tmp_progname.rbegin(), tmp_progname.rend(), '/');
  reverse_copy(tmp_progname.rbegin(), delim, back_inserter(progname));

  opt_db->set_progname(progname);
}


void
OptControl::show_help()
{
  cerr << opt_db->generate_help() << endl;
}


void
OptControl::show_usage()
{
  cerr << opt_db->generate_usage() << endl;
}

