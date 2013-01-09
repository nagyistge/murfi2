/******************************************************************************
 * RtMRIImage.h declares a class for an MR image
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#include"RtMRIImage.h"

#include"RtExperiment.h"
#include"RtDataIDs.h"

#include"ace/Log_Msg.h"
#include<fstream>

using namespace std;

// default constructor
RtMRIImage::RtMRIImage() : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage()"));

  dataID.setModuleID("mri");

  elType = RT_SHORT_TYPE;

  magicNumber = MAGIC_NUMBER;
}

// construct from an image info struct and some byte data
RtMRIImage::RtMRIImage(RtExternalImageInfo &extinfo, short *bytes)
    : RtDataImage<short>() {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtExternalImageInfo,short*)"));

  magicNumber = MAGIC_NUMBER;

  elType = RT_SHORT_TYPE;

  setInfo(extinfo);

  // fill fields of data id
  dataID.setModuleID(ID_SCANNERIMG);
  dataID.setDataName(NAME_SCANNERIMG_EPI);

  dataID.setStudyNum(getExperimentStudyID());
  dataID.setSeriesNum
      (getSeriesNumFromUID(extinfo.cSeriesInstanceUID));
  dataID.setTimePoint(extinfo.iAcquisitionNumber);

  // setup geometry
  if(getExperimentConfig().isSet("scanner:matrixSize")) {
    matrixSize = getExperimentConfig().get("scanner:matrixSize");
  }
  if(getExperimentConfig().isSet("scanner:slices")) {
    numSlices = getExperimentConfig().get("scanner:slices");
  }
  if(getExperimentConfig().isSet("scanner:voxdim1")) {
    setPixDim(0,getExperimentConfig().get("scanner:voxdim1"));
  }
  if(getExperimentConfig().isSet("scanner:voxdim2")) {
    setPixDim(1,getExperimentConfig().get("scanner:voxdim2"));
  }
  if(getExperimentConfig().isSet("scanner:voxdim3")) {
    double sliceDist = getExperimentConfig().get("scanner:voxdim3");
    if(getExperimentConfig().isSet("scanner:sliceGap")) {
      sliceDist
          += static_cast<double>(getExperimentConfig().get("scanner:sliceGap"));
    }
    setPixDim(2,sliceDist);
  }

  // allocate and copy the img data
  if(DEBUG_LEVEL & ALLOC) {
    cerr << "mr1 allocating data for " << this << endl; cerr.flush();
  }

  data = new short[numPix];

  if(bytes != NULL) {
    memcpy(data,bytes,imgDataLen);
  }
}

// construct from another image (deep copy)
// only use this with like datatypes
RtMRIImage::RtMRIImage(RtMRIImage &img) {
  ACE_TRACE(("RtMRIImage::RtMRIImage(RtMRIImage)"));

  elType = RT_SHORT_TYPE;

  (*this) = img;

  magicNumber = MAGIC_NUMBER;

  // copy the data
  if(DEBUG_LEVEL & ALLOC) {
    cerr << "mr2 allocating data for " << this << endl; cerr.flush();
  }

  data = new short[numPix];
  memcpy(data, img.data, imgDataLen);
}

// write the info (all but data) to a stream
//  in
//   stream to write to
//  out
//   success or failure
bool RtMRIImage::writeInfo(ostream &os) {
  if(os.fail()) return false;

  if(!RtDataImage<short>::writeInfo(os)) {
    return false;
  }

  // write mri fields
  os.write((char*) &slice,      sizeof(unsigned long));
  os.write((char*) &readFOV,    sizeof(double));
  os.write((char*) &phaseFOV,   sizeof(double));
  os.write((char*) &sliceThick, sizeof(double));

  char boolcon;

  boolcon = (char) swapReadPhase;
  os.write((char*) &boolcon, sizeof(char));

  unsigned int uint_tmp;

  uint_tmp = dataID.getSeriesNum();
  os.write((char*) uint_tmp, sizeof(unsigned int));

  uint_tmp = dataID.getTimePoint();
  os.write((char*) uint_tmp, sizeof(unsigned int));

  os.write((char*) &timeAfterStart, sizeof(double));
  os.write((char*) &te,             sizeof(double));
  os.write((char*) &tr,             sizeof(double));
  os.write((char*) &ti,             sizeof(double));
  os.write((char*) &triggerTime,    sizeof(double));

  char t[] = "hhmmss.xxxxxx";
  string tm = ACE_Date_Time2SiemensTime(time);
  strcpy(t,tm.c_str());
  os.write(t, SIEMENS_TIMESTR_LEN*sizeof(char));

  tm = ACE_Date_Time2SiemensTime(refFrameTime);
  strcpy(t,tm.c_str());
  os.write(t, SIEMENS_TIMESTR_LEN*sizeof(char));

  os.write((char*) &reconDelay, sizeof(double));

  boolcon = (char) distCorrect2D;
  os.write((char*) &boolcon, sizeof(char));

  boolcon = (char) moco;
  os.write((char*) &boolcon, sizeof(char));

  boolcon = (char) fromScanner;
  os.write((char*) &boolcon, sizeof(char));

  return os.good();
}

// read the info (all but data) from a stream
//  in
//   stream to read from
//  out
//   success or failure
bool RtMRIImage::readInfo(istream &is) {
  if(is.fail()) return false;

  if(!RtDataImage<short>::readInfo(is)) {
    return false;
  }

  // read mri fields
  is.read((char*) &slice,      sizeof(unsigned long));
  is.read((char*) &readFOV,    sizeof(double));
  is.read((char*) &phaseFOV,   sizeof(double));
  is.read((char*) &sliceThick, sizeof(double));

  char boolcon;

  is.read((char*) &boolcon, sizeof(char));
  swapReadPhase = (bool) boolcon;

  unsigned int uint_tmp = 0;

  is.read((char*) uint_tmp, sizeof(unsigned int));
  dataID.setSeriesNum(uint_tmp);

  is.read((char*) uint_tmp, sizeof(unsigned int));
  dataID.setTimePoint(uint_tmp);

  is.read((char*) &timeAfterStart, sizeof(double));
  is.read((char*) &te,             sizeof(double));
  is.read((char*) &tr,             sizeof(double));
  is.read((char*) &ti,             sizeof(double));
  is.read((char*) &triggerTime,    sizeof(double));

  char t[] = "hhmmss.xxxxxx";

  is.read(t, SIEMENS_TIMESTR_LEN*sizeof(char));
  time = siemensTime2ACE_Date_Time(t);

  is.read(t, SIEMENS_TIMESTR_LEN*sizeof(char));
  refFrameTime = siemensTime2ACE_Date_Time(t);

  is.read((char*) &reconDelay, sizeof(double));

  is.read((char*) &boolcon, sizeof(char));
  distCorrect2D = (bool) boolcon;

  is.read((char*) &boolcon, sizeof(char));
  moco = (bool) boolcon;

  is.read((char*) &boolcon, sizeof(char));
  fromScanner = (bool) boolcon;

  return is.good();
}


// print info about this image
void RtMRIImage::printInfo(ostream &os) {
  if(os.fail()) return;

  int wid = 30;

  os << setiosflags(ios::left);

  os << setw(wid) << "slice" << slice << endl
     << setw(wid) << "readFOV phaseFOV" << readFOV << " " << phaseFOV << endl
     << setw(wid) << "sliceThick" << sliceThick << endl
     << setw(wid) << "swapReadPhase" << swapReadPhase << endl
     << setw(wid) << "acqNum" << dataID.getTimePoint() << endl
     << setw(wid) << "timeAfterStart" << timeAfterStart << endl
     << setw(wid) << "te / tr / ti" << te << " / " << tr << " / " << ti << endl
     << setw(wid) << "triggerTime" << triggerTime << endl
     << setw(wid) << "time" << ACE_Date_Time2SiemensTime(time) << endl
     << setw(wid) << "refFrameTime" << ACE_Date_Time2SiemensTime(refFrameTime)
     << endl
     << setw(wid) << "reconDelay" << reconDelay << endl
     << setw(wid) << "distCorrect2D" << distCorrect2D << endl
     << setw(wid) << "moco" << moco << endl
     << setw(wid) << "fromScanner" << fromScanner << endl
     << setw(wid) << "MatrixSize" << getMatrixSize() << endl
     << setw(wid) << "NumSlices" << getNumSlices() << endl
     << "---------------------------" << endl;
}

// destructor
RtMRIImage::~RtMRIImage() {
  ACE_TRACE(("RtMRIImage::~RtMRIImage"));
}

// set info struct
//  in
//   _info: struct to copy
void RtMRIImage::setInfo(const RtExternalImageInfo &info) {

  // PW 2012/10/11: Trying to get murfi to save unmosaiced niftis
  if (info.iNoOfImagesInMosaic == 0) {
    // volume is not mosaiced
    dims.resize(3);
    dims[0] = info.nLin;
    dims[1] = info.nCol;
    dims[2] = info.nSli;
  } else {
    // determine the dimensions and voxel size
    dims.resize(2);
    dims[0] = info.nLin*info.iMosaicGridSize;
    dims[1] = info.nCol*info.iMosaicGridSize;
  }

  // PW 2012/10/16: This is most definitly the WRONG place to put this... But
  //                for some reason the constructor was getting called *after*
  //                setInfo(), so sliceGap wasn't being set correctly.
  if(getExperimentConfig().isSet("scanner:sliceGap")) {
    sliceGap = getExperimentConfig().get("scanner:sliceGap");
  }

  pixdims.resize(3);
  pixdims[0] = info.dFOVread / info.nLin;
  pixdims[1] = info.dFOVphase / info.nCol;
  pixdims[2] = info.dThick / info.nSli * (1+sliceGap);

  // set geometry info
  setPixDim(0,pixdims[0]);
  setPixDim(1,pixdims[1]);
  setPixDim(2,pixdims[2]);

  // calculate image size
  imgDataLen = bytesPerPix;
  numPix = 1;
  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    imgDataLen *= (*i);
    numPix *= (*i);
  }

  // scaling matrix
  vnl_matrix_fixed<double,4,4> scaleMat;
  scaleMat.set_identity();
  scaleMat.put(0,0, pixdims[0]);
  scaleMat.put(1,1, pixdims[1]);
  scaleMat.put(2,2, pixdims[2]);

  // rotation matrix
  vnl_matrix_fixed<double,4,4> rotMat;
  rotMat.set_identity();

  rotMat.put(0,0, info.dRowSag);
  rotMat.put(1,0, info.dRowCor);
  rotMat.put(2,0, info.dRowTra);

  rotMat.put(0,1, info.dColSag);
  rotMat.put(1,1, info.dColCor);
  rotMat.put(2,1, info.dColTra);

  rotMat.put(0,2, info.dNorSag);
  rotMat.put(1,2, info.dNorCor);
  rotMat.put(2,2, info.dNorTra);

  // PW 2012/10/03: Seimens' logical coordinate system is LPS
  // we want coords in RAS
  vnl_matrix_fixed<double,4,4> lps2ras;
  lps2ras.set_identity();
  lps2ras.put(0,0,-1);
  lps2ras.put(1,1,-1);

  vxl2ras = (lps2ras*rotMat)*scaleMat;

  // PW 2012/10/03: Calculating offset to center of k-space
  // See
  // http://www.nmr.mgh.harvard.edu/~rudolph/software/vox2ras/download/vox2ras_ksolve.html
  // With a slight modification.  Since info.dPos.. is the offset to slice
  // #zero, we don't need to traverse in the slice direction (we are already
  // there) only in the phase encode and readout directions This should
  // probably tested with a few more volumes (and definitly with patient
  // orientations other than HFS)
  vnl_matrix_fixed<double,3,1> Vc_x = vxl2ras.extract(3,1,0,0);
  vnl_matrix_fixed<double,3,1> Vc_y = vxl2ras.extract(3,1,0,1);
  vnl_matrix_fixed<double,3,1> Vc_z = vxl2ras.extract(3,1,0,2);
  vnl_matrix_fixed<double,3,1> Vc_Ps;
  double xoff = info.nLin / 2.0;
  double yoff = info.nCol / 2.0;
  //double zoff = info.iMosaicGridSize / 2.0;
  Vc_Ps.put(0,0, info.dPosSag);
  Vc_Ps.put(1,0, info.dPosCor);
  Vc_Ps.put(2,0, info.dPosTra);
  vnl_matrix_fixed<double,3,1> Vc_Pe1 =
      lps2ras.extract(3,3)*(Vc_Ps + (xoff*Vc_x + yoff*Vc_y));
  vnl_matrix_fixed<double,3,1> Vc_Pe2 =
      lps2ras.extract(3,3)*(Vc_Ps - (xoff*Vc_x + yoff*Vc_y));
  vxl2ras.put(0,3, Vc_Pe1.get(0,0));
  vxl2ras.put(1,3, Vc_Pe1.get(1,0));
  vxl2ras.put(2,3, Vc_Pe2.get(2,0));

  // debugging
  //cout << "scale" << endl;
  //printVnl44Mat(scaleMat);
  //cout << "rot" << endl;
  //printVnl44Mat(rotMat);
  //cout << "vxl2ras" << endl;
  //printVnl44Mat(vxl2ras);

  // build RAS 2 REF transformation matrix
  // PW 2012/10/12 TODO!
  ras2ref.set_identity();

  // image info
  slice = info.lSliceIndex;
  readFOV = info.dFOVread;
  phaseFOV = info.dFOVphase;
  matrixSize = info.nCol;
  numSlices = info.iNoOfImagesInMosaic;
  sliceThick = info.dThick;
  seriesInstanceUID = info.cSeriesInstanceUID;

  swapReadPhase = info.bSwapReadPhase;
  dataID.setTimePoint(info.iAcquisitionNumber);
  timeAfterStart = info.dTimeAfterStart;
  te = info.dTE;
  tr = info.dTR;
  ti = info.dTI;
  triggerTime = info.dTriggerTime;

  // actual acquision info parms
  time = siemensTime2ACE_Date_Time(info.chAcquisitionTime);
  refFrameTime = siemensTime2ACE_Date_Time(info.chframeOfReference);
  reconDelay = info.dTimeDelay;

  // scanner online post-processing parms
  distCorrect2D = false;
  moco = info.bIsMoCo;

  // received data parms
  fromScanner = info.iDataSource == 0;
}

// set the matrix size
void RtMRIImage::setMatrixSize(unsigned int ms) {
  matrixSize = ms;
}

// get the matrix size
unsigned int RtMRIImage::getMatrixSize() {
  return matrixSize;
}

// set the matrix size
void RtMRIImage::setNumSlices(unsigned int ns) {
  numSlices = ns;
}

// get the matrix size
unsigned int RtMRIImage::getNumSlices() {
  return numSlices;
}

// get a smart contrast level
float RtMRIImage::getAutoContrast() {
  ACE_TRACE(("RtMRIImage::getAutoContrast"));

  if(!minMaxSet) {
    setMinMax();
  }

  return SHRT_MAX/(float) maxVal;
}


// get a smart brightness level
float RtMRIImage::getAutoBrightness() {
  ACE_TRACE(("RtMRIImage::getAutoBrightness"));

  if(!minMaxSet) {
    setMinMax();
  }

  return (float) minVal;

}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
