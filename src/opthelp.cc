// opthelp.cc

#include <algorithm>
#include <limits>
#include "opthelp.hh"
#include "optexcept.hh"

using namespace optparser;
using std::string;
using std::map;
using std::list;
using std::make_pair;
using std::bad_alloc;
using std::numeric_limits;


// InfoGroup:
//   Member functions.
//
InfoGroup::~InfoGroup()
{
  for (std::map<OptID, OptInfo*>::const_iterator i = oi_units.begin(); i != oi_units.end(); ++i) {
    delete i->second;
  }
}


bool
InfoGroup::add_unit(const OptID& oid, const std::string& ldesc, const std::string& desc)
{
  bool result = false;
  map<OptID, OptInfo*>::const_iterator i = oi_units.find(oid);
  if (i == oi_units.end()) {
    OptInfo* unit = 0;
    try {
      unit = new OptInfo(ldesc, desc);
    }
    catch (...) {
      throw optparser_error("Memory allocation failed, 'OptInfo'.");
    }
    oi_units.insert(make_pair(oid, unit));
    result = true;
  }
  return result;
}


bool
InfoGroup::get_optid_list(std::list<OptID>* oidlist) const
{
  bool result = false;
  if (oidlist) {
    for (map<OptID, OptInfo*>::const_iterator i = oi_units.begin(); i != oi_units.end(); ++i) {
      oidlist->push_back(i->first);
    }
    result = true;
  }
  return result;
}


std::string
InfoGroup::get_lopt_desc(const OptID& oid) const
{
  map<OptID, OptInfo*>::const_iterator i = oi_units.find(oid);
  if (i == oi_units.end()) {
    return string();
  }
  return i->second->lopt_desc();
}


std::string
InfoGroup::get_desc(const OptID& oid) const
{
  map<OptID, OptInfo*>::const_iterator i = oi_units.find(oid);
  if (i == oi_units.end()) {
    return string();
  }
  return i->second->desc();
}


size_t
InfoGroup::get_lopt_desc_length(const OptID& oid) const
{
  map<OptID, OptInfo*>::const_iterator i = oi_units.find(oid);
  if (i == oi_units.end()) {
    return 0;
  }
  return i->second->lopt_desc_len();
}


size_t
InfoGroup::get_desc_length(const OptID& oid) const
{
  map<OptID, OptInfo*>::const_iterator i = oi_units.find(oid);
  if (i == oi_units.end()) {
    return 0;
  }
  return i->second->desc_len();
}


// OptDBHelp:
//   Member functions.
//
OptDBHelp::OptDBHelp(size_t col_size): AbstractDecoratorOptDB(create_db()),
				       help_caption("Usage: "),
				       width_limit(col_size),
				       first_col_width(0),
				       second_col_width(0),
				       third_col_width(0)
{
  OptID gid(numeric_limits<unsigned int>::max());
  InfoGroup* grp = 0;
  try {
    grp = new InfoGroup(string("Help options"));
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'InfoGroup'.");
  }
  oi_grps.insert(make_pair(gid, grp));
}


OptDBHelp::~OptDBHelp()
{
  for (map<OptID, InfoGroup*>::const_iterator i = oi_grps.begin(); i != oi_grps.end(); ++i) {
    delete i->second;
  }
}


void
OptDBHelp::set_progname(const std::string& pgname)
{
  string tmp = pgname + ' ';
  copy(tmp.begin(), tmp.end(), back_inserter(help_caption));
}


void
OptDBHelp::set_caption(const std::string& capstr)
{
  copy(capstr.begin(), capstr.end(), back_inserter(help_other_caption));
}


OptID
OptDBHelp::set_opt_info(const OptID& grpid,
			char sopt,
			const char* lopt,
			const char* ldesc,
			const char* desc)
{
  std::map<OptID, InfoGroup*>::const_iterator ig = oi_grps.find(grpid);
  if (ig == oi_grps.end()) {
    throw optparser_error("This group is not found.");
  }
  InfoGroup* grp = ig->second;

  OptID oid = set_opt_data(sopt, lopt);

  string lopt_desc;
  string description;
  if (ldesc) {
    lopt_desc = ldesc;
  }
  if (desc) {
    description = desc;
  }

  if (! grp->add_unit(oid, lopt_desc, description)) {
    throw optparser_error("Duplicate GroupID.");
  }
  return oid;
}


OptID
OptDBHelp::add_group(const char* grpdesc)
{
  OptID gid = alloc_grpid();
  InfoGroup* grp = 0;
  try {
    if (grpdesc) {
      grp = new InfoGroup(string(grpdesc));
    }
    else {
      grp = new InfoGroup();
    }
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'InfoGroup'.");
  }

  oi_grps.insert(make_pair(gid, grp));
  return gid;
}


OptID
OptDBHelp::get_help_group_id() const 
{
  OptID help_gid(numeric_limits<unsigned int>::max());
  std::map<OptID, InfoGroup*>::const_iterator i = oi_grps.find(help_gid);
  if (i == oi_grps.end()) {
    throw optparser_error("Help group is not found.");
  }
  return help_gid;
}


std::string
OptDBHelp::generate_help()
{
  // Calculation of the length of the option columns.
  calc_help_col_width();

  // To create the help massage.
  string help = help_caption + help_other_caption + "\n\n";

  for (map<OptID, InfoGroup*>::const_iterator i = oi_grps.begin(); i != oi_grps.end(); ++i) {
    if (i->second->get_size()) {
      help += gen_group_help(i->second);
      help.push_back('\n');
    }
  }

  return help;
}


std::string
OptDBHelp::generate_usage()
{
  // Calculation of the length of the option columns.
  calc_usage_col_width();

  // To create the help massage.
  string title = help_caption;
  string usage;
  
  for (map<OptID, InfoGroup*>::const_iterator i = oi_grps.begin(); i != oi_grps.end(); ++i) {
    usage += gen_group_usage(i->second);
    usage.push_back('\n');
  }

  if (usage.empty()) {
    usage = title;
  }
  else {
    usage.replace(0, first_col_width, title);
  }
  return usage;
}


// OptDBHelp:
//   Private member functions.
// 
OptID
OptDBHelp::alloc_grpid()
{
  unsigned char seed = 0;
  for (map<OptID, InfoGroup*>::const_iterator i = oi_grps.begin(); i != oi_grps.end(); ++i) {
    if (seed > i->first.number()) {
      seed = i->first.number();
    }
  }
  if (seed == numeric_limits<unsigned int>::max() - 1) {
    throw optparser_error("The number of OptID reached the upper limit.");
  }
  return OptID(seed + 1);
}


void
OptDBHelp::calc_help_col_width()
{
  // Short option part.
  first_col_width = c_sopt_col_width;

  // Long option part.
  size_t max_lopt_len = 0;

  for (map<OptID, InfoGroup*>::const_iterator i = oi_grps.begin(); i != oi_grps.end(); ++i) {
    list<OptID> id_list;
    if (! i->second->get_optid_list(&id_list)) {
      continue;
    }

    for (list<OptID>::const_iterator id = id_list.begin(); id != id_list.end(); ++id) {
      size_t lopt_len = get_lopt_length(*id);
      if (! lopt_len) {
	continue;
      }
      lopt_len += i->second->get_lopt_desc_length(*id);

      if (lopt_len > max_lopt_len) {
	max_lopt_len = lopt_len;
      }
    }
  }

  if (first_col_width + c_lopt_col_padding + max_lopt_len > width_limit / 2) {
    second_col_width = width_limit / 2;
  }
  else {
    second_col_width = c_lopt_col_padding + max_lopt_len;
  }

  // Description part.
  third_col_width = width_limit - (first_col_width + second_col_width);
}


void
OptDBHelp::calc_usage_col_width()
{
  first_col_width = help_caption.size();
  second_col_width = width_limit - first_col_width;
  third_col_width = 0;
}


std::string
OptDBHelp::gen_group_help(InfoGroup* grp)
{
  string msg = grp->get_group_desc() + ":\n";
  string indent(first_col_width + second_col_width, ' ');

  list<OptID> oidlist;
  if (! grp->get_optid_list(&oidlist)) {
    throw optparser_error("Failed to get the list of OptID.");
  }

  for (list<OptID>::const_iterator id = oidlist.begin(); id != oidlist.end(); ++id) {
    string sopt_part;
    if (get_sopt_length(*id) != 0) {
      sopt_part = "  " + get_sopt_name(*id);
      if (get_lopt_length(*id) != 0) {
	sopt_part += ",";
      }
      sopt_part += string(first_col_width - sopt_part.size(), ' ');
    }
    else {
      sopt_part = string(first_col_width, ' ');
    }
    copy(sopt_part.begin(), sopt_part.end(), back_inserter(msg));
    
    string lopt_part;
    if (get_lopt_length(*id) != 0) {
      lopt_part = get_lopt_name(*id);
      if (grp->get_lopt_desc_length(*id) != 0) {
	lopt_part += "=" + grp->get_lopt_desc(*id);
      }
    }

    if (sopt_part.size() + lopt_part.size() > first_col_width + second_col_width) {
      lopt_part.push_back('\n');
      copy(indent.begin(), indent.end(), back_inserter(lopt_part));
    }
    else {
      lopt_part += string(second_col_width - lopt_part.size(), ' ');
    }
    copy(lopt_part.begin(), lopt_part.end(), back_inserter(msg));
      
    string desc_part;
    if (grp->get_desc_length(*id) != 0) {
      string desc = grp->get_desc(*id);
      string::const_iterator b = desc.begin();
      string::const_iterator e;

      for (e = b; e != desc.end(); ++e) {
	if (*e == '\n' || e == b + third_col_width) {
	  copy(b, e, back_inserter(msg));
	  msg += '\n';
	  b = (*e == '\n') ? e + 1 : e;
	  copy(indent.begin(), indent.end(), back_inserter(msg));
	}
      }
      copy(b, e, back_inserter(desc_part));
      desc_part += '\n';
    }
    else {
      desc_part = string(third_col_width, ' ') + '\n';
    }
    copy(desc_part.begin(), desc_part.end(), back_inserter(msg));
  }

  return msg;
}


std::string
OptDBHelp::gen_group_usage(InfoGroup* grp)
{
  string indent(first_col_width, ' ');
  string msg = indent;
  
  list<OptID> oidlist;
  if (! grp->get_optid_list(&oidlist)) {
    throw optparser_error("Failed to get the list of OptID.");
  }

  size_t len = 0;
  for (list<OptID>::const_iterator id = oidlist.begin(); id != oidlist.end(); ++id) {
    // To create the sentence of the option. (ex. [-x|--y=ZZZZ])
    string sentence("[");
    if (get_sopt_length(*id)) {
      sentence += get_sopt_name(*id);
      if (get_lopt_length(*id)) {
	sentence += '|';
      }
    }
    if (get_lopt_length(*id)) {
      sentence += get_lopt_name(*id);
      if (grp->get_lopt_desc_length(*id)) {
	sentence += '=' + grp->get_lopt_desc(*id);
      }
    }
    sentence.push_back(']');

    // Checking for the line width.
    if (len + sentence.size() + 1 < second_col_width) {
      len += sentence.size() + 1;
    }
    else {
      msg.push_back('\n');
      copy(indent.begin(), indent.end(), back_inserter(msg));
      len = sentence.size() + 1;
    }

    copy(sentence.begin(), sentence.end(), back_inserter(msg));
    msg.push_back(' ');
  }

  return msg;
}

