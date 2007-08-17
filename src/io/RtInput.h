/******************************************************************************
 * RtInput.h declares a class that implements input operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTINPUT_H
#define RTINPUT_H

#include <sigc++/sigc++.h>

#include"RtIO.h"
#include"RtData.h"
#include"RtConfig.h"

// class declaration
class RtInput : public RtIO {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInput(); 

  // destructor
  virtual ~RtInput();

  // init
  virtual bool init(const RtConfig &config);

private:

  //*** private data members  ***//

  // callback to receive the input
  sigc::signal<void, RtData&> callback;
  

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


