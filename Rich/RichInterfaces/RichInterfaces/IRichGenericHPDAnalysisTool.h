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

//-----------------------------------------------------------------------------
/** @file IRichGenericHPDAnalysisTool.h
 *
 *  Header file for tool interface : Rich::IGenericHPDAnalysisTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   10/10/2009
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichSmartID.h"

namespace Rich {
  //-----------------------------------------------------------------------------
  /** @class IGenericHPDAnalysisTool RichKernel/IRichGenericHPDAnalysisTool.h
   *
   *  Interface for tools that perform any data anlysis on a single HPD data block
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   10/10/2009
   */
  //-----------------------------------------------------------------------------

  class IGenericHPDAnalysisTool : public virtual IAlgTool {

  public:
    /// Interface ID
    DeclareInterfaceID( IGenericHPDAnalysisTool, 1, 0 );

  public:
    /** @class Result RichKernel/IRichGenericHPDAnalysisTool.h
     *
     *  Results of the HPD analysis
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   10/10/2009
     */
    class Result {
    public:
      LHCb::RichSmartID id;      ///< Detector component the result refers to
      std::string       message; ///< Message associated with the result
      StatusCode        status;  ///< StatusCode indicating the severity of the report
    };

    /// Type for a list of results
    using Results = std::vector<Result>;

  public:
    /** Applies HPD data analysis to given HPD data
     *
     *  @param hpdID    HPD identifier
     *  @param smartIDs Reference to vector of pixel RichSmartIDs for the given HPD
     *
     *  @return StatusCode indicating if the analysis was successfully run or not
     */
    virtual StatusCode analyse( const LHCb::RichSmartID hpdID, const LHCb::RichSmartID::Vector& smartIDs,
                                IGenericHPDAnalysisTool::Results& results ) const = 0;
  };

} // namespace Rich
