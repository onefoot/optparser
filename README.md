optparser
=========

This program allows parsing the command line options and displaying the Help message automatically.

Usage:
------

 See "optparser_example.cc".



*Example:*
<pre>
$ ./optpex --help
Usage: optpex [OPTIONS]

Options:
  -x, --first=INT          first option.
  -y, --second=STRING      second option.
  -z, --third              third option.
  -a, --fourth=FLOAT       fourth option.
  -b, --fifth=DOUBLE       fifth option.
  -c, --sixth              sixth option.

Help options:
  -?, --help               Show this help message.
      --usage              Show usage message.

</pre>

<pre>
$ ./optpex --help
Usage: optpex [-x|--first=INT] [-y|--second=STRING] [-z|--third]
              [-a|--fourth=FLOAT] [-b|--fifth=DOUBLE] [-c|--sixth]
              [-?|--help] [--usage]

</pre>
