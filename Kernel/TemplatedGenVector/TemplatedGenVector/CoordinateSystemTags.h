// @(#)root/mathcore:$Id$
// Authors: W. Brown, M. Fischler, L. Moneta    2005

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG LHCbROOT MathLib Team and                     *
  *                      FNAL LCG LHCbROOT MathLib Team                    *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header source file for CoordinateSystemTag's classes
//
// Created by: Lorenzo Moneta  at Wed Apr 05 2006
//
//

#ifndef LHCbROOT_Math_GenVector_CoordinateSystemTags
#define LHCbROOT_Math_GenVector_CoordinateSystemTags  1



namespace LHCbROOT {

namespace Math {


//__________________________________________________________________________________________
   /**
      DefaultCoordinateSystemTag
      Default tag for identifying any coordinate system

      @ingroup GenVector
   */

   class  DefaultCoordinateSystemTag {};


//__________________________________________________________________________________________
   /**
      Tag for identifying vectors based on a global coordinate system

      @ingroup GenVector
   */
   class  GlobalCoordinateSystemTag {};

//__________________________________________________________________________________________
   /**
      Tag for identifying vectors based on a local coordinate system

      @ingroup GenVector
   */
   class   LocalCoordinateSystemTag {};


}  // namespace Math

}  // namespace LHCbROOT



#endif
