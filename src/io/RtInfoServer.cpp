/******************************************************************************
 * RtInfoServer.cpp declares a class for serving info about real to clients
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-02-11
 * 
 *****************************************************************************/

#include"RtInfoServer.h"
#include"RtExperiment.h"
#include"RtEvent.h"
#include "RtDataStore.h"
#include "RtDesignMatrix.h"

#include"RtDataIDs.h"
#include"tinyxml/tinyxml.h"
#include<vnl/vnl_matrix.h>

static char *VERSION = "$Id$";

const unsigned short DEFAULT_PORT_NUM = 15001;

// default constructor
RtInfoServer::RtInfoServer() : RtServerSocket(DEFAULT_PORT_NUM) {
    addToID(":infoserver");

    lastGoodTriggerTR = -1;
    lastBadTriggerTR = -1;

  posAct = new RtData*[1024];
  negAct = new RtData*[1024];
} 
  
// constructor with port and host

RtInfoServer::RtInfoServer(unsigned short portNum) : RtServerSocket(portNum) {
    addToID(":infoserver");

    lastGoodTriggerTR = -1;
    lastBadTriggerTR = -1;

  posAct = new RtData*[1024];
  negAct = new RtData*[1024];
}

// destructor

RtInfoServer::~RtInfoServer() {
    delete [] posAct;
}

// open and start accepting
bool RtInfoServer::open(RtConfig &config) {
  // find port
  if(config.isSet("infoserver:port")) {
    address.set_port_number((unsigned short) config.get("infoserver:port"));
  }

  numPos = 1;
  numNeg = 1;

  return RtServerSocket::open(config);
}

// set some data
// NOTE: at the moment this only takes activation sums and stores them for 
// each tr in an xml document

void RtInfoServer::setData(RtData *data) {

  //cout << "EVENT TRIGGER SETDATA " << data->getDataID() << endl;
  if(data->getDataID().getRoiID() 
     == getExperimentConfig().get("infoserver:posActivationDataID").str()) {
    //    cout << "!! FOUND pos !!!!!!!!!!!!!!!!!" << endl;
    posAct[data->getDataID().getTimePoint()] = data;
    numPos++;
  }
  else if(data->getDataID().getRoiID() 
     == getExperimentConfig().get("infoserver:negActivationDataID").str()) {
    negAct[data->getDataID().getTimePoint()] = data;
    numNeg++;
  }
  //else if(data->getDataID().getModuleID() == ID_EVENTTRIGGER) {

        // note about "good" and "bad" triggers: these are just to denote two
        // types of triggers, one based on activation in the direction you
        // expect, and the other opposite. neither is an error

    else if (data->getDataID().getDataName() == NAME_EVENTTRIGGER_BAD) { // bad trigger
        lastBadTriggerTR = data->getDataID().getTimePoint();

        //cout << "!! FOUND BAD TRUGGER !!!!!!!!!!!!!!!!!" << endl;

    }
    else if (data->getDataID().getDataName() == NAME_EVENTTRIGGER_GOOD) { // otherwise its good
        lastGoodTriggerTR = data->getDataID().getTimePoint();
        //cout << "!! FOUND BAD TRUGGER !!!!!!!!!!!!!!!!!" << endl;
    }
    else {
        cout << "infoserver: ignoring a " << data->getDataID() << endl;
    }
}

// receive an XML message
// in 
//  string recieved
//  stream recieved on
// out XML string response

string RtInfoServer::receiveMessage(string &message, ACE_SOCK_Stream &stream) {
    cout << message << endl;

    // set up the response
    TiXmlDocument response;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
    response.LinkEndChild(decl);

    // parse the request
    TiXmlDocument request;
    request.Parse(message.c_str());
    if (request.Error()) {
        string errString = "could not parse request XML";
        cerr << errString << endl;
        // put an unsupported msg in XML
        TiXmlElement *infoResponse = new TiXmlElement("info");
        response.LinkEndChild(infoResponse);
        TiXmlElement *errEl = createErrorElement(errString);
        infoResponse->LinkEndChild(errEl);
        // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
    }

    for (TiXmlNode *info = 0; (info = request.IterateChildren("info", info));) {
        // create an info node for this request
        TiXmlElement *infoResponse = new TiXmlElement("info");
        response.LinkEndChild(infoResponse);

        // look for gets
        for (TiXmlElement *get = 0; (get = (TiXmlElement*) info->IterateChildren("get", get));) {

            // check name TODO figure out a way to check the data id string?
            //            if (strcmp(get->Attribute("dataid"), "dataid")) {
            //                // put an unsupported msg in XML
            //                TiXmlElement *errEl = createErrorElement("unsupported data type");
            //                infoResponse->LinkEndChild(errEl);
            //                // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
            //                continue;
            //            }

            //            // create a designmatrix node for this request
            //            TiXmlElement *designmatrixResponse = new TiXmlElement("designmatrix");
            //            infoResponse->LinkEndChild(designmatrixResponse);

            // create data id object from input attribute
            string dataIDString = get->Attribute("dataid");
            RtDataID dataID;
            dataID.setFromString(dataIDString);

            // retrieve from data store using data id from input string
            RtData *retrievedData = getDataStore().getData(dataID);

            // check for NULL
            if (retrievedData == NULL) {
                cerr << "Data from data store is NULL." << endl;
                continue;
            }

            // call serializeAsXML for retrievedData class
            TiXmlElement *dataElement = retrievedData->serializeAsXML();

            // set data id attribute and link to inforesponse
            dataElement->SetAttribute("dataid", dataID.toString());
            infoResponse->LinkEndChild(dataElement);

        }

        // look for sets
        for (TiXmlElement *set = 0; (set = (TiXmlElement*) info->IterateChildren("set", set));) {
            // check name TODO figure out a way to check the data id string?
            //            if (strcmp(set->Attribute("name"), "dataid")) {
            //                // put an unsupported msg in XML
            //                TiXmlElement *errEl = createErrorElement("unsupported data type");
            //                infoResponse->LinkEndChild(errEl);
            //                // I THINK WE DON'T FREE THE ELEMENTS, BUT CHECK THIS
            //                continue;
            //            }

            // create data id object from input attribute
            string dataIDString = set->Attribute("dataid");
            RtDataID dataID;
            dataID.setFromString(dataIDString);

            // retrieve from data store using data id from input string
            RtData *retrievedData = getDataStore().getData(dataID);

            // check for NULL
            if (retrievedData == NULL) {
                cerr << "Data from data store is NULL." << endl;
                continue;
            }

            // call unserializeXML for retrievedData class
            retrievedData->unserializeXML(set->FirstChildElement());
        }
    }
    return buildXMLString(response);
}

// build a string from an XML document
// in
//  XML document 
// out
//  string representation

string RtInfoServer::buildXMLString(TiXmlDocument &doc) {
    // get string version of xml
    TiXmlPrinter printer;
    printer.SetStreamPrinting();
    doc.Accept(&printer);
    return printer.Str();
}

// build an error element
// in
//  name of the error
// out
//  XML element containing error info

TiXmlElement *RtInfoServer::createErrorElement(string error) {
    TiXmlElement *errEl = new TiXmlElement("data");
    errEl->SetAttribute("name", "error");
    errEl->SetAttribute("type", error);
    return errEl;
}

// get the version
//  out: char array that represents the cvs version

char *RtInfoServer::getVersionString() {
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
