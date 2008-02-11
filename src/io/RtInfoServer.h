/******************************************************************************
 * RtInfoSever.h declares a class for serving info about real to clients
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 * 
 *****************************************************************************/

#ifndef RTINFOSERVER_H
#define RTINFOSERVER_H

#include"RtOutput.h"
#include"RtServerSocket.h"
#include<vector>
using namespace std;

// class declaration
class RtInfoServer : public RtOutput, public RtServerSocket  {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInfoServer(); 
  
  // constructor with port and host
  RtInfoServer(unsigned short portNum, string hostname = ""); 

  // destructor
  virtual ~RtInfoServer();

  // hand of some data to be output
  virtual void setData(RtData *data);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  // receive a message
  // in 
  //  string recieved
  //  stream recieved on
  // out  success or failure 
  virtual string recieveMessage(string &message, ACE_SOCK_Stream &stream);

  // build a string from an XML document
  // in
  //  XML document 
  // out
  //  string representation
  string buildXMLString(TiXmlDocument &doc);

  // database
  // NOTE: now this only stores activation sums by tr.
  // TODO: reconsider how to store old data and manage access to it
  vector<RtData*> database;
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

