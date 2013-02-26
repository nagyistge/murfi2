/*=========================================================================
 *  RtDiff.h is the header for a class that computes the difference
 *  between two images
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

#ifndef RTDIFF_H
#define RTDIFF_H

#include"RtStreamComponent.h"
#include"RtMRIImage.h"

// class declaration
class RtDiff : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtDiff();

  // destructor
  virtual ~RtDiff();

 protected:

  // validate the configuration
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // pointer to the last data we got
  RtMRIImage *last;

};

#endif
