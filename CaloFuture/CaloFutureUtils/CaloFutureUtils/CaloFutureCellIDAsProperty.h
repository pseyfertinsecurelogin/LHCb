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
// ============================================================================
#pragma once
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <iosfwd>
#include <map>
#include <string>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"     // for GAUDI_API  // IWYU pragma: keep
#include "GaudiKernel/StatusCode.h" // IWYU pragma: keep
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h" // IWYU pragma: keep
// ============================================================================
/** @file
 *  Streamer& Parsing function to allow CaloCellID & related classes to be
 *  used as properties for Gaudi components
 *  @see class LHCb::CaloCellID
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-09-29
 */
// ============================================================================
//                                                                      Parsers
// ============================================================================
namespace Gaudi {
  namespace Parsers {
    // ========================================================================
    /** parse cellID from the string
     *  @param result (OUPUT) the parsed cellID
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    StatusCode parse( LHCb::CaloCellID& result, const std::string& input );
    // ========================================================================
    /** parse the vector of cellIDs from the string
     *  @param result (OUPUT) the parsed vector of cellIDs
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    GAUDI_API
    StatusCode parse( LHCb::CaloCellID::Vector& result, const std::string& input );
    // ========================================================================
    /** parse the vector of cellIDs from the string
     *  @param result (OUPUT) the parsed vector of cellIDs
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    GAUDI_API
    StatusCode parse( LHCb::CaloCellID::Set& result, const std::string& input );
    // ========================================================================
    /** parse the map of  { cellID : double } from the string
     *  @param result (OUPUT) the parsed map { cellID : double }
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    GAUDI_API
    StatusCode parse( std::map<LHCb::CaloCellID, double>& result, const std::string& input );
    // ========================================================================
    /** parse the map of  { cellID : vector<double> } from the string
     *  @param result (OUPUT) the parsed map { cellID : vector<double> }
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    GAUDI_API
    StatusCode parse( std::map<LHCb::CaloCellID, std::vector<double>>& result, const std::string& input );
    // ========================================================================
  } // namespace Parsers
} // namespace Gaudi
// ============================================================================
//                                                      Another name for parser
// ============================================================================
namespace CaloFutureCellCode {
  // ==========================================================================
  class GAUDI_API Cell2String {
  public:
    // ========================================================================
    /** parse cellID from the string
     *  @param result (OUPUT) the parsed cellID
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static StatusCode cellFromString( LHCb::CaloCellID& result, const std::string& input );
    // ========================================================================
    /** parse cellIDs from the string
     *  @param result (OUPUT) the parsed cellIDs
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static StatusCode cellFromString( LHCb::CaloCellID::Set& result, const std::string& input );
    // ========================================================================
    /** parse cellIDs from the string
     *  @param result (OUPUT) the parsed cellIDs
     *  @param input  (INPUT) the input string
     *  @return status code
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static StatusCode cellFromString( LHCb::CaloCellID::Vector& result, const std::string& input );
    // ========================================================================
    /** convert cellID into string
     * @param  cell (INPUT) cell to be converted
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static std::string cellToString( const LHCb::CaloCellID& cell );
    // ========================================================================
    /** convert cellIDs into string
     *  @param  cells (INPUT) cells to be converted
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static std::string cellToString( const LHCb::CaloCellID::Set& cells );
    // ========================================================================
    /** convert cellIDs into string
     *  @param  cells (INPUT) cells to be converted
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-09-29
     */
    static std::string cellToString( const LHCb::CaloCellID::Vector& cells );
    // ========================================================================
  };
  // ==========================================================================
} //                                              end of namespace CaloFutureCellCode
// ============================================================================
// The END
// ============================================================================
