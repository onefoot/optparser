// optid.hh

#ifndef __OPTID_HH__
#define __OPTID_HH__

namespace optparser {

  // OptID:
  //   The option ID object.
  //
  class OptID {
    unsigned char idn;

public:
    // constructor & destructor
    OptID(): idn(0) {}
    OptID(unsigned char num): idn(num) {}
    OptID(const OptID& src): idn(src.idn) {}

    // method
    unsigned char number() const { return idn; }

    // operator
    const OptID& operator=(const OptID& src) { idn = src.idn; return *this; }
    bool operator==(const OptID& src) const { return idn == src.idn; }
    bool operator!=(const OptID& src) const { return ! operator==(src); }
    bool operator<(const OptID& src) const { return idn < src.idn; }
    bool operator<=(const OptID& src) const { return idn <= src.idn; }
    bool operator>(const OptID& src) const { return idn > src.idn; }
    bool operator>=(const OptID& src) const { return idn >= src.idn; }
    operator bool () { return idn != 0; }

  };

}


#endif
