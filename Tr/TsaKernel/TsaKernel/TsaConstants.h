#ifndef _TsaConstants_H
#define _TsaConstants_H 1

// CLHEP
#include "GaudiKernel/PhysicalConstants.h"

/** @namespace TsaConstants TsaConstants.h Tsa/TsaConstants.h
 *
 *  Namespace for some useful type definitions and const
 *
 *  @author M.Needham
 *  @date   13/3/2002
*/

namespace TsaConstants{

 const double zMagnet = 500.0*Gaudi::Units::cm;
 const double endTracker = 950.0*Gaudi::Units::cm;
 const double beginTracker = 700.0*Gaudi::Units::cm; 
 
 const double pMax = 500.0*Gaudi::Units::GeV;
 const double pMin = 100.0*Gaudi::Units::MeV;

};

#endif //  _TsaConstants_H
