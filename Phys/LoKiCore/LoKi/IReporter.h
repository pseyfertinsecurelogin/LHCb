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
#ifndef LOKI_IREPORTER_H
#define LOKI_IREPORTER_H 1
// ============================================================================
// Include files
// ============================================================================
// from GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/IErrorTool.h"
// ============================================================================
// forward declarations
// ============================================================================
class GaudiException ;
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class IReporter IReporter.h LoKi/IReporter.h
   *
   *  @author Vanya BELYAEV ivelyaev@physics.syr.edu
   *  @date   2003-01-16
   */
  class IReporter : public extend_interfaces<IErrorTool>
  {
  public:
    DeclareInterfaceID( IReporter , 5 , 0 ) ;
    // ========================================================================
  public:
    // ========================================================================
    /// define maximal number of prints
    static  void setMaxErrorPrint   ( const std::size_t ) ; // max prints
    /// define maximal number of warning prints
    static  void setMaxWarningPrint ( const std::size_t ) ; // max prints
    // ========================================================================
  public:
    // ========================================================================
    /// maximal number of error   prints
    static std::size_t maxErrorPrint   () ;
    /// maximal number of warning prints
    static std::size_t maxWarningPrint () ;
    // ========================================================================
  private:
    // ========================================================================
    /// maximal number of error prints
    static std::size_t s_maxErrorPrint   ;
    /// maximal number of warning prints
    static std::size_t s_maxWarningPrint ;
    // ========================================================================
  } ;
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_IREPORTER_H
