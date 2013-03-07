// optvalfact.cc

#include "optvalfact.hh"
#include "optval.hh"
#include "optexcept.hh"

using namespace optparser;
using std::string;


// OptValFact:
//   Static member functions.
//
OptVal*
OptValFact::conv(int* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueInt(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueInt'.");
  }
  return ov;
}


OptVal*
OptValFact::conv(float* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueFloat(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueFloat'.");
  }
  return ov;
}


OptVal*
OptValFact::conv(double* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueDouble(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueDouble'.");
  }
  return ov;
}


OptVal*
OptValFact::conv(std::string* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueString(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueString'.");
  }
  return ov;
}


OptVal*
OptValFact::conv(char* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueChar(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueChar'.");
  }
  return ov;
}


OptVal*
OptValFact::conv(bool* dst)
{
  OptVal* ov = 0;
  try {
    ov = new ValueBool(dst);
  }
  catch (...) {
    throw optparser_error("Memory allocation failed, 'ValueBool'.");
  }
  return ov;
}

