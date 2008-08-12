/******************************************************************************
 * RtProcessor.cpp is the definition of a class for a processing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id: RtProcessor.cpp 107 2008-04-18 02:00:17Z ohinds $";

#include"RtProcessor.h"
#include"RtDataIDs.h"
#include"RtDiff.h"
#include"RtVar.h"

#include"tinyxml/tinyxml.h"

// default constructor
RtProcessor::RtProcessor() 
  : RtStreamComponent() {
  setID("RtProcessor");
}

// destructor
RtProcessor::~RtProcessor() {
  
}

//*** initialization routines  ***//

// add modules based on the processor configuration
//  in
//   config
//  out
//   success or failure
int RtProcessor::addModules(RtConfig &config) {
  ACE_TRACE(("RtProcessor::addModules"));

  // make sure the stack is empty
  while(!addMod.empty()) {
    addMod.pop();
  }

  // get the node for preprocessing 
  TiXmlNode *procNode = config.getNode("processor");

  // check for preprocessor node (backward compatibility)
  if(procNode == NULL) {
    procNode = config.getNode("preprocessor");
  }  

  // if no preprocessing was specified, just pass the data
  if(procNode == NULL || procNode->Type() != TiXmlNode::ELEMENT) {    
    if(config.getConductor()->getDisplay() != NULL) {
      addSingleModule(RtPasser::moduleString, "original data passer")
	->addOutput(config.getConductor()->getDisplay());
    }
  }
  else { // find modules in the node and add each
    addModulesFromNode((TiXmlElement*) procNode, &config);
  }
    // all modules are now added to the stream
  pushAllModules();


  return 0;
}

// process a single acquisition
int RtProcessor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtProcessor::process"));

  // pass the message block down to our stream
  RtStream::put(mb);  

  return 0;
}

// get the version
//  out: char array that represents the cvs version
char *RtProcessor::getVersionString() {
  return VERSION;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

