/******************************************************************************
 * RtData.h declares a base class for data classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTDATA_H
#define RTDATA_H

#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;

// class declaration
class RtData {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtData();

  // destructor
  virtual ~RtData();

protected:

  //*** data members  ***//
  
  ptime creationTime;

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


