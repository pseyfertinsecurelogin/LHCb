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
/** @file RichCommonBase.h
 *
 *  Header file for RICH base class : Rich::CommonBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2005-08-27
 */
//-----------------------------------------------------------------------------

#pragma once

// Gaudi
#include "Gaudi/Accumulators.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SerializeSTL.h"

// Utils
#include "RichUtils/RichSIMDTypes.h"

// Vc
#include <Vc/common/alignedbase.h>

// Forward declarations
class IJobOptionsSvc;
class DeRich;

// Some defines for debug/verbose messages...
#ifndef _ri_debug
#  define _ri_debug                                                                                                    \
    if ( this->msgLevel( MSG::DEBUG ) ) this->debug()
#endif
#ifndef _ri_verbo
#  define _ri_verbo                                                                                                    \
    if ( this->msgLevel( MSG::VERBOSE ) ) this->verbose()
#endif

namespace Rich {
  using GaudiUtils::operator<<;

  namespace Future {

    //-----------------------------------------------------------------------------
    /** @class CommonBase RichCommonBase.h RichFutureKernel/RichCommonBase.h
     *
     *  Base class providing common functionality for all RICH tools and algorithms
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   2005-08-27
     */
    //-----------------------------------------------------------------------------

    template <class PBASE>
    class CommonBase : public PBASE, public Vc::AlignedBase<Vc::VectorAlignment> {

    public:
      // Inherit base class constructors
      using PBASE::PBASE;

    public:
      /** Initialization of the algorithm after creation
       *
       * @return The status of the initialization
       * @retval StatusCode::SUCCESS Initialization was successful
       * @retval StatusCode::FAILURE Initialization failed
       */
      virtual StatusCode initialize() override;

      /** Finalization of the algorithm before deletion
       *
       * @return The status of the finalization
       * @retval StatusCode::SUCCESS Finalization was successful
       * @retval StatusCode::FAILURE Finalization failed
       */
      virtual StatusCode finalize() override;

    public:
      /** Returns the full location of the given object in the Data Store
       *
       *  @param pObj Data object
       *
       *  @return Location of given data object
       */
      inline std::string objectLocation( const DataObject* pObj ) const {
        return ( !pObj ? "Null DataObject !" : ( pObj->registry() ? pObj->registry()->identifier() : "UnRegistered" ) );
      }

      /** @brief Forced release of a particular tool
       *
       *  Tools are automatically released during finalisation, so this method
       *  only need be used to release a tool early, before finalisation.
       *
       *  @param pTool  Pointer to the tool to be released
       */
      template <typename TOOL>
      inline void releaseTool( TOOL*& pTool ) const {
        if ( pTool ) {
          if ( UNLIKELY( this->msgLevel( MSG::DEBUG ) ) ) {
            this->debug() << " Forced release for tool '" << pTool->name() << "'" << endmsg;
          }
          this->release( pTool );
          pTool = nullptr;
        } else {
          this->warning() << "Attempt to release a NULL Tool pointer" << endmsg;
        }
      }

      /// Load an object from the given store
      template <typename TOOL, typename SERVICE>
      decltype( auto ) acquire( SERVICE svc, std::string loc ) {
        DataObject* tmp = nullptr;
        auto        sc  = svc->retrieveObject( std::move( loc ), tmp );
        return ( sc ? dynamic_cast<TOOL*>( tmp ) : nullptr );
      }

    protected:
      // definitions

      using WarningCounter = Gaudi::Accumulators::MsgCounter<MSG::WARNING>; ///< Warning counter
      using ErrorCounter   = Gaudi::Accumulators::MsgCounter<MSG::ERROR>;   ///< Error counter
    };

  } // namespace Future
} // namespace Rich
