/******************************************************************************
 * RtVar.h is the implementation of a class that computes the variance
 * of a set of images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtVar.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtVar::moduleString("voxel-variance");

// default constructor
RtVar::RtVar() : RtStreamComponent() {
  numTimePoints = 0;
  mean.addToID("voxel_variance");
  id = moduleString;
  varnum = NULL;
}

// destructor
RtVar::~RtVar() {
  cout << "destroyed" << endl;

  if(varnum != NULL) {
    delete [] varnum;
  }
}

// process a single acquisition
int RtVar::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtVar::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtDataImage *img = (RtDataImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtVar:process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtVar:process: image passed is NULL\n"));
    return 0;
  }

  if(numTimePoints == 0 || mean.getSeriesNum() != img->getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "var found first image\n"));

    mean = (*img);

    varnum = new double[mean.getNumPix()];
    for(int i = 0; i < mean.getNumPix(); i++) {
      varnum[i] = 0;
    }

    numTimePoints++;

    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "including image %d in the variance estimate\n", 
	     img->getAcquisitionNum()));
  
  // validate sizes
  if(img->getNumPix() != mean.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtVar:process: last image is different size than this one\n"));
    return -1;    
  }
  
  // save the data
  numTimePoints++;


  // allocate a new data image for the variance
  RtDataImage *var = new RtDataImage(img->getInfo());
  
  // update the mean and variance numerator due to west (1979) for each voxel 
  for(int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) mean.getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
      + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    mean.setPixel(i, (unsigned short) newmean);
  
    double pixvarnum = varnum[i];
    double newvarnum = pixvarnum 
      + (numTimePoints-1) * (thispix - pixmean) 
      * (thispix - pixmean) / (double) numTimePoints;
    varnum[i] = newvarnum;

    var->setPixel(i, (unsigned short) (newvarnum / (double)(numTimePoints-1)));
  }  

  // set the image id for handling
  var->addToID("voxel_variance");
  setResult(msg,var);

  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
