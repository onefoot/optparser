// basicoptdb.cc

#include "basicoptdb.hh"
#include "optexcept.hh"

using namespace optparser;
using std::string;
using std::map;
using std::make_pair;
using std::bad_alloc;


// OptDB:
//   Static member functions.
//
OptDB*
OptDB::create_db()
{
  OptDB* pdb;
  try {
    pdb = new BasicOptDB;
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'BasicOptDB.");
  }
  return pdb;
}


// OptUnit:
//   Member functions.
//
bool
OptUnit::check_opt(const std::string& optname)
{
  bool result = false;
  if (short_name.compare(optname) == 0) {
    result = true;
  }
  else if (long_name.compare(optname) == 0) {
    result = true;
  }
  return result;
}
  

// BasicOptDB:
//   Member functions.
//
BasicOptDB::~BasicOptDB()
{
  for (map<OptID, OptUnit*>::const_iterator i = o_units.begin(); i != o_units.end(); ++i) {
    delete i->second;
  } 
  delete last_allocated_id;
}
  
  
OptID
BasicOptDB::set_opt_data(char sopt, const char* lopt)
{
  string short_opt;
  string long_opt;

  // Checking for the format of the options.
  if (! sopt && ! lopt) {
    throw optparser_error("You must specify the short or long option.");
  }

  // Checking for the option name.
  if (sopt) {
    short_opt.push_back('-');
    short_opt.push_back(sopt);

    if (find_opt(short_opt)) {
      string emsg("Duplicate option, '");
      throw optparser_error(emsg + sopt + "'.");
    }
  }
  if (lopt) {
    long_opt = lopt;
    if (long_opt.size() < 2) {
      string emsg("The name of the long option is shortage, '");
      throw optparser_error(emsg + lopt + "'.");
    }
    long_opt = string("--") + long_opt;
    if (find_opt(long_opt)) {
      string emsg("Duplicate option, '");
      throw optparser_error(emsg + lopt + "'.");
    }
  }

  OptID* new_id = alloc_optid();
  OptUnit* new_unit = 0;
  try {
    new_unit = new OptUnit(short_opt, long_opt);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'OptUnit'.");
  }
    
  o_units.insert(make_pair(*new_id, new_unit));
  return *new_id;
}

  
OptID
BasicOptDB::find_opt(const std::string& optname) const
{
  if (! optname.size()) {
    return OptID();
  }
    
  for (map<OptID, OptUnit*>::const_iterator i = o_units.begin(); i != o_units.end(); ++i) {
    if (i->second->check_opt(optname)) {
      return i->first;
    }
  } 
  return OptID();
}
  

std::string
BasicOptDB::get_sopt_name(const OptID& oid) const
{
  map<OptID, OptUnit*>::const_iterator i = o_units.find(oid);
  if (i == o_units.end()) {
    return string();
  }
  return i->second->sname();
}


std::string
BasicOptDB::get_lopt_name(const OptID& oid) const 
{
  map<OptID, OptUnit*>::const_iterator i = o_units.find(oid);
  if (i == o_units.end()) {
    return string();
  }
  return i->second->lname();
}


size_t
BasicOptDB::get_sopt_length(const OptID& oid) const
{
  map<OptID, OptUnit*>::const_iterator i = o_units.find(oid);
  if (i == o_units.end()) {
    return 0;
  }
  return i->second->sname_len();
}

size_t
BasicOptDB::get_lopt_length(const OptID& oid) const
{
  map<OptID, OptUnit*>::const_iterator i = o_units.find(oid);
  if (i == o_units.end()) {
    return 0;
  }
  return i->second->lname_len();
}


// BasicOptDB:
//   Private member functions.
// 
OptID*
BasicOptDB::alloc_optid()
{
  OptID* new_id = 0;
  try {
    if (! last_allocated_id) {
      new_id = new OptID(1);
    }
    else {
      new_id = new OptID(last_allocated_id->number() + 1);
    }
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'OptID'.");
  }

  delete last_allocated_id;
  last_allocated_id = new_id;
  return new_id;
}

