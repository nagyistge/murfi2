/******************************************************************************
 * RtEventTriggerActivationDiff.h is the header for a class to
 * trigger an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 
 *
 *****************************************************************************/

#ifndef RTEVENTTRIGGERACTIVATIONDIFF_H
#define RTEVENTTRIGGERACTIVATIONDIFF_H

#include"RtEventTrigger.h"

// class declaration
class RtEventTriggerActivationDiff : public RtEventTrigger {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTriggerActivationDiff();

  // destructor
  ~RtEventTriggerActivationDiff();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  double diffThresh;
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

