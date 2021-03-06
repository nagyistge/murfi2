/*=========================================================================
 *  c++ adaptation for gentleman's least squares computation
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include"RtLeastSquaresSolve.h"
#include"debug_levels.h"

#include<cstring>
#include<iostream>
using namespace std;

// constructors
RtLeastSquaresSolve::RtLeastSquaresSolve() {
  numMeas = 1;
  numCols = 2;
  init();
}

// with the number of columns in the design
RtLeastSquaresSolve::RtLeastSquaresSolve(unsigned int _numCols) {
  numCols = _numCols;
  numMeas = 1;
  init();
}

// with the number of columns in the design and the number of
// observations at each time
RtLeastSquaresSolve::RtLeastSquaresSolve(unsigned int _numCols,
                                         unsigned int _numMeas) {
  numCols = _numCols;
  numMeas = _numMeas;
  init();
}

// destructor
RtLeastSquaresSolve::~RtLeastSquaresSolve() {
  for(unsigned int i = 0; i < numMeas; i++) {
    delete d[i];
    delete rbar[i];
    delete thetabar[i];
  }
  delete d;
  delete rbar;
  delete thetabar;
  delete sserr;
}

// initialize the variables
void RtLeastSquaresSolve::init() {
  // allocate dim 1
  d = new double*[numMeas];
  rbar = new double*[numMeas];
  thetabar = new double*[numMeas];
  sserr = new double[numMeas];

  // allocate dim 2
  for(unsigned int i = 0; i < numMeas; i++) {
    d[i] = new double[numCols];
    rbar[i] = new double[numCols*(numCols-1)/2];
    thetabar[i] = new double[numCols];

    // initialize
    for(unsigned int j = 0; j < numCols; j++) {
      d[i][j] = 0.0;
      thetabar[i][j] = 0.0;
    }

    for(unsigned int j = 0; j < numCols*(numCols-1)/2; j++) {
      rbar[i][j] = 0.0;
    }
  }
}

// include a new observation in the estimate
// in:
//  yelem  observation vector at this timepoint
//  xrow   design matrix row (must be p elements long)
//  weight of this observation
//
// translated from Algorithm AS 75.1 Appl. Statist. (1974) Vol.23, No.3, p448
bool RtLeastSquaresSolve::include(double *yelems, double *Xrow, double weight) {
  double y, di, xi, xk, dpi, cbar, sbar, *xrow;
  int nextr;

  if(weight == 0.0) { // yes, i know this is stupid, thank you
    return true;
  }

  // iterate over data
  for(unsigned int i = 0; i < numMeas; i++) {
    y = yelems[i];

    // copy in the row to prevent modification
    xrow = new double[numCols];
    memcpy(xrow,Xrow,numCols*sizeof(double));

    for(unsigned int j = 0; j < numCols; j++) {
      /*     Skip unnecessary transformations.  Test on exact zeros must */
      /*     be used or stability can be destroyed. */
      if(weight == 0.0 || xrow[j] == 0.0) {
        continue;
      }
      xi = xrow[j];
      di = d[i][j];
      dpi = di + weight * xi * xi;
      cbar = di / dpi;
      sbar = weight * xi / dpi;
      weight = cbar * weight;
      d[i][j] = dpi;
      if(j != numCols-1) {
        nextr = j*(numCols + numCols - (j+1)) / 2;
        for(unsigned int k=j+1; k < numCols; k++, nextr++) {
          xk = xrow[k];
          xrow[k] = xk - xi * rbar[i][nextr];
          rbar[i][nextr] = cbar * rbar[i][nextr] + sbar * xk;
        }
      }

      xk = y;
      y = xk - xi * thetabar[i][j];
      thetabar[i][j] = cbar * thetabar[i][j] + sbar * xk;
    }

    sserr[i] += weight * y * y;

    delete xrow;
  }
  return true;
}

// regress using the model with data up to this point
// out:
//  vector of betas
//
// adapted from Algorithm AS 75.4  Appl. Statist. (1974), Vol. 23, No. 3, p448
double *RtLeastSquaresSolve::regress(unsigned int meas) {
  if(meas < 0 || meas >= numMeas) {
    return NULL;
  }

  double *beta = new double[numCols];

  for(unsigned int j = 0; j < numCols; j++) {
    int ind = numCols-j-1;
    beta[ind] = thetabar[meas][ind];
    int nextr = ind*(numCols + numCols - (ind+1)) / 2;
    for(unsigned int k = ind+1; k < numCols; k++, nextr++) {
      beta[ind] -= rbar[meas][nextr] * beta[k];
    }
  }
  return beta;
}

// compute and return the squared error residual for each column
//
// adapted from Algorithm AS75.3  Appl. Statist. (1974) Vol.23, No. 3, P448
double *RtLeastSquaresSolve::getSquaredError(unsigned int meas) {
  if(meas < 0 || meas >= numMeas) {
    return NULL;
  }

  double *ss = new double[numCols];
  for(unsigned int i = 0; i < numCols; ++i) {
    ss[i] = d[meas][i] * (thetabar[meas][i] * thetabar[meas][i]);
  }
  return ss;
}

// compute and return the total squared error
double RtLeastSquaresSolve::getTotalSquaredError(unsigned int meas) {
  if(meas < 0 || meas >= numMeas) {
    return 0.0;
  }
  return sserr[meas];
}
