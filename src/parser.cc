// parser.cc

#include <algorithm>
#include <map>
#include <set>
#include "parser.hh"
#include "ioptcmd.hh"
#include "optexcept.hh"

using namespace optparser;
using std::string;
using std::list;
using std::map;
using std::set;
using std::pair;
using std::bad_alloc;
using std::not_equal_to;

// Parser:
//   Member functions.
// 
bool
Parser::parse(char** src_argv, size_t n_argv)
{
  if (! src_argv || n_argv < 1) {
    return false;
  }
  for (size_t i = 0; i != n_argv; ++i) {
    if (! src_argv[i]) {
      throw optparser_error("Invalid parameters for parsing.");
    }
  }

  // Parsing.
  // Phase1
  assort_type(src_argv, n_argv);

  // Phase2
  assort_opt_val();

  // Phase3
  analyze_opt();
  
  // Phase4
  check_duplicate();

  // Checking Help and Usage.
  for (list<ParseUnit>::const_iterator i = parsed_args.begin(); i != parsed_args.end(); ++i) {
    if (i->cls == CLS_HELP) {
      optcmd->set_help_enable();
      return false;
    }
    if (i->cls == CLS_USAGE) {
      optcmd->set_usage_enable();
      return false;
    }
  }

  // To apply the values and leftover arguments.
  map<OptID, string> optvals;
  for (list<ParseUnit>::const_iterator i = parsed_args.begin(); i != parsed_args.end(); ++i) {
    if (i->type != OAT_VALUE && i->cls == CLS_NORMAL) {
      optvals.insert(make_pair(i->oid, i->value));
    }
  }
  optcmd->set_parsed_values(optvals);
  optcmd->set_leftover_args(lo_args);
  return true;
}


// Parser:
//   Private member functions.
// 
void
Parser::assort_type(char** src_argv, size_t n_argv)
{
  for (size_t i = 0 ; i != n_argv; ++i) {
    ParseUnit uni;
    uni.type = check_arg_type(src_argv[i]);
    uni.src = src_argv[i];
    parsed_args.push_back(uni);
  }
}


void
Parser::assort_opt_val()
{
  list<ParseUnit>::iterator i = parsed_args.begin();
  while (i != parsed_args.end()) {
    string option, value;
    
    switch (i->type) {
    case OAT_SHORT:
    case OAT_LONG:
      option = i->src;
      break;
    case OAT_LONG_COMBINED:
      if (! divide_combined_arg(i->src, '=', &option, &value)) {
	goto err;
      }
      if (check_arg_type(value.c_str()) != OAT_VALUE) {
	goto err;
      }
      break;
    case OAT_VALUE:
      value = i->src;
      break;
    default:			// OAT_UNKNOWN
      goto err;
    }

    if (! option.empty()) {
      i->oid = optcmd->get_optid(option);
      if (! i->oid) {
	goto err;
      }
    }
    if (! value.empty()) {
      i->value = value;
    }
    ++i;
  }
  return;
  
 err:
  throw optparser_error(string("Invalid option. '") + i->src + "'");
}


void
Parser::analyze_opt()
{
  for (list<ParseUnit>::iterator i = parsed_args.begin(); i != parsed_args.end(); ++i) {
    // Checking leftover argument.
    if (i->type == OAT_VALUE) {
      lo_args.push_back(i->value);
      continue;
    }

    // Checking the class of option.
    if (optcmd->check_help_opt(i->oid)) {
      i->cls = CLS_HELP;
    }
    else if (optcmd->check_usage_opt(i->oid)) {
      i->cls = CLS_USAGE;
    }
    else {
      i->cls = CLS_NORMAL;
    }

    // If Help or Usage, bypass to check the value.
    if (i->cls == CLS_HELP || i->cls == CLS_USAGE) {
      if (i->value.empty()) {
	continue;
      }
      throw optparser_error(string("Invalid option. '") + i->src + "'");
    }
      
    // Checking the value and merging into the option.
    if (optcmd->check_has_value(i->oid)) {
      if (i->value.empty()) {
	if (++i == parsed_args.end()) {
	  throw optparser_error(string("The parameter is not enough, '") + (--i)->src + "'.");
	}
	if (i->type != OAT_VALUE) {
	  throw optparser_error(string("The parameter is not enough, '") + (--i)->src + "'.");
	}
	string next_val = i->value;
	(--i)->value = next_val;
	++i;
      }
    }
    else {
      if (! i->value.empty()) {
	throw optparser_error(string("Invalid option. '") + i->src + "'");
      }
    }
  }
}


void
Parser::check_duplicate()
{
  set<OptID> tmp_ids;
  
  for (list<ParseUnit>::const_iterator i = parsed_args.begin(); i != parsed_args.end(); ++i) {
    if (i->type != OAT_VALUE) {
      pair<set<OptID>::iterator, bool> res = tmp_ids.insert(i->oid);
      if (! res.second) {
	throw optparser_error(string("Duplicate option. '") + i->src + "'");
      }
    }
  }
}


Parser::OPTARG_TYPE
Parser::check_arg_type(const char* src_arg)
{
  if (! src_arg) {
    return OAT_UNKNOWN;
  }

  string arg(src_arg);
  string::iterator i = find_if(arg.begin(), arg.end(), bind1st(not_equal_to<char>(), '-'));

  OPTARG_TYPE type;
  switch (i - arg.begin()) {
  case 0:			// value
    type = OAT_VALUE;
    break;
  case 1:			// short option
    type = arg.size() == 2 ? OAT_SHORT : OAT_UNKNOWN;
    break;
  case 2:			// long option
    type = arg.size() > 3 ? OAT_LONG : OAT_UNKNOWN;
    break;
  default:
    type = OAT_UNKNOWN;
  }
  
  if (type == OAT_LONG) {
    string::iterator p = find(i, arg.end(), '=');
    if (p != arg.end()) {
      if (p == i || (p + 1) == arg.end() || (p - i) == 1 || *(p + 1) == '=') {
	type = OAT_UNKNOWN;
      }
      else {
	type = OAT_LONG_COMBINED;
      }
    }
  }

  return type;
}


bool
Parser::divide_combined_arg(const char* src_arg,
			    const char sep,
			    std::string* dst_opt,
			    std::string* dst_val)
{
  if (! src_arg) {
    return false;
  }
  
  string src(src_arg);
  
  string::iterator opt_end = find(src.begin(), src.end(), sep);
  if (opt_end == src.begin() || opt_end == src.end()) {
    return false;
  }
  string::iterator val_begin = find_if(opt_end, src.end(), bind1st(not_equal_to<char>(), sep));
  if (val_begin - opt_end > 1 || val_begin == src.end()) {
    return false;
  }

  copy(src.begin(), opt_end, back_inserter(*dst_opt));
  copy(val_begin, src.end(), back_inserter(*dst_val));
  return true;
}
