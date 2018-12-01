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
#include "DetDesc/ParamAsHisto.h"

//----------------------------------------------------------------------------
namespace DetDesc {
  namespace Params {
    //------------------------------------------------------------------------
    /// Get the value of TH1D parameter of a Condition
    const Histo1D *paramAsHisto1D(const Condition *cond, const std::string &name){
      return cond ? &( cond->param<DetDesc::Params::Histo1D>(name) ) : NULL;
    }
    //------------------------------------------------------------------------
    /// Get the value of TH2D parameter of a Condition
    const Histo2D *paramAsHisto2D(const Condition *cond, const std::string &name){
      return cond ? &( cond->param<DetDesc::Params::Histo2D>(name) ) : NULL;
    }
  }
}
//----------------------------------------------------------------------------

