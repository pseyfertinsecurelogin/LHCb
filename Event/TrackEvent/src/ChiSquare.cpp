/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// local
#include "Event/ChiSquare.h"
#include "gsl/gsl_cdf.h"

namespace LHCb {
  double ChiSquare::prob() const {
    double val( 0 );
    if ( nDoF() > 0 ) {
      const double limit   = 1e-15;
      double       chi2max = gsl_cdf_chisq_Qinv( limit, nDoF() );
      val                  = chi2() < chi2max ? gsl_cdf_chisq_Q( chi2(), nDoF() ) : 0;
    }
    return val;
  }
} // namespace LHCb
