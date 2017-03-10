// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG LHCbROOT FNAL MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

//
// Created by: Mark Fischler Tues July 19  2005
//

#include "TemplatedGenVector/GenVector_exception.h"

namespace LHCbROOT {
namespace Math {
bool GenVector_exception::fgOn = false;

void Throw(GenVector_exception & e) { if (GenVector_exception::fgOn) throw e; }


void GenVector::Throw(const char * s) {
   if (!GenVector_exception::fgOn) return;
   GenVector_exception e(s);
   throw e;
}



}  // namespace Math
}  // namespace LHCbROOT
