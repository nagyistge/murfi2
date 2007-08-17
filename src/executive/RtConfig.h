/******************************************************************************
 * RtConfig.h is the header for a class that controls configuration of
 * a a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONFIG_H
#define RTCONFIG_H


#include<cctype>
#include<cmath>
#include<string>
#include<algorithm>
#include<string>
#include<cstring>
#include<sstream>
#include<map>

using namespace std;

// class to wrap string for a single parm value
class RtConfigVal;

// class declaration
class RtConfig {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConfig(); 

  // constructor for bool assignment
  RtConfig(bool b); 

  // destructor
  ~RtConfig();

  //*** config loading routines ***//

  // parse sommand line args
  bool parseArgs(int argc, char **args);

  // parse config file
  bool parseConfigFile();

  //*** config get/set parms ***/

  // get a parm value
  RtConfigVal &get(string name);

  // set parm
  template<class T>
  void set(string name, T tval);

private:

  // map of config params
  map<string,RtConfigVal> parms;

  // prints the usage info for the realtime system
  void printUsage();

  // print the name/value pairs to the screen
  void dumpConfig();

};


class RtConfigVal {
  
public:

  //*** constructor ***/

  // leave blank
  RtConfigVal() { 
    val = "";
  }

  // assign it straight off
  inline RtConfigVal(string s) {
    val=s;
    //transform(val.begin(),val.end(),val.begin(),(int(*)(int))tolower); 
  }

  //*** operators ***//

  // logical not
  const bool operator!() {
    if(
       val == "0"
       || val == "false"
       || val == ""
       ) {
      return true;
    }
    return false;
  }

  // assignment
  template<class T>
  RtConfigVal &operator=(const T t) {
    stringstream ss;
    ss << t;
    ss >> val;
    return *this;
  }

  // comparison 
  const bool operator==(const string s) {
    return val == s;
  }

  const bool operator==(const double d) {
    double dval;
    return convert<double>(dval,val) 
      & fabs(d - dval) < dTol;
  }

  const bool operator==(const float f) {
    float fval;
    return convert<float>(fval,val) 
      & fabsf(f - fval) < fTol;
  }

  const bool operator==(const long l) {
    long lval;
    return convert<long>(lval,val) 
      & l == lval;
  }

  const bool operator==(const int i) {
    int ival;
    return convert<int>(ival,val) 
      & i == ival;
  }

  const bool operator==(const bool b) {
    bool bval;
    return convert<bool>(bval,val) 
      & bval == b;
  }

  // outstream operator
 friend ostream &operator<<(ostream &os, const RtConfigVal &v) {
    os << v.val;
    return os;
  }

private:

  string val;

  // tolerances for double and float comparison
  static const float fTol = 0.00000000001f;
  static const double dTol = 0.00000000001;

  // for conversion of string types to other types
  template <class T> inline bool convert(T& t, const string& s) {
    istringstream iss(s);
    return !(iss >> dec >> t).fail();
  }
};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


