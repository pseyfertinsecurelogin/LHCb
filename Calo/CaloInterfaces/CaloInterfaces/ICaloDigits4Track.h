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
#ifndef CALOINTERFACES_ICALODIGITS4TRACK_H 
#define CALOINTERFACES_ICALODIGITS4TRACK_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <set>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/CaloCellID.h"
// ============================================================================
// DigiEvent 
// ============================================================================
#include "Event/CaloDigit.h"
#include "Event/Track.h"
// ============================================================================
// LHCbMath 
// ============================================================================
#include "LHCbMath/Line.h"
// ============================================================================
// forward decalrations
// ============================================================================
namespace LHCb
{
  class CaloDigit ;
}
// ============================================================================
/** @class ICaloDigits4Track CaloInterfaces/ICaloDigits4Track.h
 *  
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-10-25
 */
struct GAUDI_API ICaloDigits4Track : extend_interfaces<IAlgTool>
{
  // ==========================================================================
  /// the actual type of line in 3D  
  typedef Gaudi::Math::Line<Gaudi::XYZPoint,Gaudi::XYZVector>       Line      ;
  // ==========================================================================
  // ==========================================================================
  /** collect the cellID-s along the line 
   *  @param line   (INPUT)  the line  
   *  @param cells  (OUTPUT) the container of cells 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const Line&               line   , 
    LHCb::CaloCellID::Vector& cells  ) const = 0 ;
  // ==========================================================================
  /** collect the cellID-s along the line 
   *  @param line   (INPUT)  the line  
   *  @param cells  (OUTPUT) the container of cells 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const Line&               line   , 
    LHCb::CaloCellID::Set&    cells  ) const = 0 ;
  // ==========================================================================  
  /** collect the cellID-s along the path of the tracks 
   *  @param track  (INPUT)  the track 
   *  @param cells  (OUTPUT) the container of cells 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const LHCb::Track*        track  , 
    LHCb::CaloCellID::Vector& cells  ) const = 0 ;
  // ==========================================================================
  /** collect the cellID-s along the path of the tracks 
   *  @param track  (INPUT)  the track 
   *  @param cells  (OUTPUT) the container of cells 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const LHCb::Track*        track  , 
    LHCb::CaloCellID::Set&    cells  ) const = 0 ;
  // ==========================================================================  
  /** collect the fired digits along the line 
   *  @param line   (INPUT)  the line  
   *  @param digits (OUTPUT) the container of digits 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const Line&               line   , 
    LHCb::CaloDigit::Vector&  digits ) const = 0 ;
  // ==========================================================================
  /** collect the fired digits along the path of the tracks 
   *  @param line   (INPUT)  the line  
   *  @param digits (OUTPUT) the container of digits 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const Line&               line   , 
    LHCb::CaloDigit::Set&     digits ) const = 0 ;
  // ==========================================================================  
  /** collect the fired digits along the path of the tracks 
   *  @param track  (INPUT)  the track 
   *  @param digits (OUTPUT) the container of digits 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const LHCb::Track*                   track  , 
    LHCb::CaloDigit::Vector& digits ) const = 0 ;
  // ==========================================================================
  /** collect the fired digits along the path of the tracks 
   *  @param track  (INPUT)  the track 
   *  @param digits (OUTPUT) the container of digits 
   *  @return status code 
   */
  virtual StatusCode collect 
  ( const LHCb::Track*        track  , 
    LHCb::CaloDigit::Set&     digits ) const  = 0 ;
  // ==========================================================================  
  /** static interface identification
   *  @see IInterface
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloDigits4Track , 2 , 0 ) ;
  // ==========================================================================  
};
// ============================================================================
// The END 
// ============================================================================
#endif // CALOINTERFACES_ICALODIGITS4TRACK_H
// ============================================================================
