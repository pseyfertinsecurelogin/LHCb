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
#define _ri_debug                                                                                  \
  if ( msgLevel( MSG::DEBUG ) ) debug()
#endif
#ifndef _ri_verbo
#define _ri_verbo                                                                                  \
  if ( msgLevel( MSG::VERBOSE ) ) verbose()
#endif

namespace Rich
{
  using GaudiUtils::operator<<;

  namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class CommonBase RichCommonBase.h RichFutureKernel/RichCommonBase.h
     *
     *  Base class providing common functionality for all RICH tools and algorithms
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   2005-08-27
     */
    //-----------------------------------------------------------------------------

    template < class PBASE >
    class CommonBase : public PBASE, public Vc::AlignedBase< Vc::VectorAlignment >
    {

    public:

      /// Standard algorithm-like constructor
      CommonBase( const std::string &name, ISvcLocator *pSvcLocator );

      /// Standard tool-like constructor
      CommonBase( const std::string &type, const std::string &name, const IInterface *parent );

      /// Standard Converter-like Constructor
      CommonBase( long storage_type, const CLID &class_type, ISvcLocator *svc = nullptr );

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
      inline std::string objectLocation( const DataObject *pObj ) const
      {
        return ( !pObj ? "Null DataObject !" :
                         ( pObj->registry() ? pObj->registry()->identifier() : "UnRegistered" ) );
      }

      /** @brief Forced release of a particular tool
       *
       *  Tools are automatically released during finalisation, so this method
       *  only need be used to release a tool early, before finalisation.
       *
       *  @param pTool  Pointer to the tool to be released
       */
      template < typename TOOL >
      inline void releaseTool( TOOL *&pTool ) const
      {
        if ( pTool )
        {
          if ( UNLIKELY( this->msgLevel( MSG::DEBUG ) ) )
          { this->debug() << " Forced release for tool '" << pTool->name() << "'" << endmsg; }
          this->release( pTool );
          pTool = nullptr;
        }
        else
        {
          this->Warning( "Attempt to release a NULL Tool pointer" );
        }
      }

    protected: // methods

      /// Pointer to Job Options Service
      inline IJobOptionsSvc *joSvc() const noexcept { return m_jos; }

      /** Propagate a list oj job options from one object to another
       *  @param from_name The name of the object to get the options from
       *  @param to_name   The name of the oject to copy the options to
       *  @param options   List of options to copy. If empty, all options are copied.
       *  @param overwrite If true, options will be over-written in the target object
       *                   if they are already set
       *  @return StatusCode indicating if the options where correctly copied
       */
      StatusCode
      propagateJobOptions( const std::string &               from_name,
                           const std::string &               to_name,
                           const std::vector< std::string > &options = std::vector< std::string >(),
                           const bool                        overwrite = false ) const;

    private:

      /// Common Constructor initisalisations
      void initRichCommonConstructor();

    private: // data

      /// Pointer to job options service
      IJobOptionsSvc *m_jos = nullptr;
    };

  } // namespace Future
} // namespace Rich
