#include <iostream>
#include <string>
#include <list>
#include "src/optparser.hh"

using namespace optparser;
using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv)
{
  int n = 0;
  float f = 0;
  double d = 0;
  string s;
  bool b = false;
  char c = 0;
  
  OptControl* ocon = new OptControl;
  try {
    OptID grp1 = ocon->add_group("Options");

    ocon->set_option(grp1, OptValFact::conv(&n), 'x', "first", "INT", "first option.");
    ocon->set_option(grp1, OptValFact::conv(&s), 'y', "second", "STRING", "second option.");
    ocon->set_option(grp1, OptValFact::conv(&b), 'z', "third", 0, "third option.");
    ocon->set_option(grp1, OptValFact::conv(&f), 'a', "fourth", "FLOAT", "fourth option.");
    ocon->set_option(grp1, OptValFact::conv(&d), 'b', "fifth", "DOUBLE", "fifth option.");
    ocon->set_option(grp1, OptValFact::conv(&c), 'c', "sixth", 0, "sixth option.");

    ocon->set_help_option('?', "help", "Show this help message.");
    ocon->set_usage_option(0, "usage", "Show usage message.");
    ocon->set_help_caption("[OPTIONS]");
  
    if (! ocon->parse(argc, argv)) {
      delete ocon;
      return 1;
    }
  }
  catch (const optparser_error& e) {
    cerr << e.what() << endl;
    delete ocon;
    return -1;
  }
      
  cout << "Value:" << endl;
  cout << " n: " << n << endl;
  cout << " f: " << f << endl;
  cout << " d: " << d << endl;
  cout << " s: " << s << endl;
  cout << " b: " << b << endl;
  cout << " c: " << c << endl;

  cout << "Leftover arguments:" << endl;;
  list<string> leftover_args = ocon->get_leftover_args();
  for (list<string>::iterator i = leftover_args.begin(); i != leftover_args.end(); ++i) {
    cout << *i << endl;
  }
  
  delete ocon;
  return 0;
}
