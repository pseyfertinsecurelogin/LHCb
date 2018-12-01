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
/** @file RichCommonConstructors.cpp
 *
 *  Implementation of templated constructors for Rich::CommonBase
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2004-09-18
 */
//-----------------------------------------------------------------------------

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiAlg/GaudiTupleTool.h"

// local
#include "RichFutureKernel/RichCommonBase.h"
#include "RichFutureKernel/RichConverter_Imp.h"
#include "RichFutureKernel/RichHistoBase.h"

namespace Rich
{
  namespace Future
  {

    //=============================================================================
    // Implementation of RichCommonBase for GaudiAlgorithm
    //=============================================================================
    template <>
    CommonBase< GaudiAlgorithm >::CommonBase( const std::string &name, ISvcLocator *pSvcLocator )
      : GaudiAlgorithm( name, pSvcLocator )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiAlgorithm >::CommonBase( const std::string & /* type */,
                                              const std::string & /* name */,
                                              const IInterface * /* parent */ )
      : GaudiAlgorithm( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiAlgorithm> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiAlgorithm >::CommonBase( long /* storage_type */,
                                              const CLID & /* class_type */,
                                              ISvcLocator * /* svc */ )
      : GaudiAlgorithm( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiAlgorithm> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //=============================================================================

    //=============================================================================
    // Implementation of RichCommonBase for GaudiHistoAlg
    //=============================================================================
    template <>
    CommonBase< GaudiHistoAlg >::CommonBase( const std::string &name, ISvcLocator *pSvcLocator )
      : GaudiHistoAlg( name, pSvcLocator )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiHistoAlg >::CommonBase( const std::string & /* type */,
                                             const std::string & /* name */,
                                             const IInterface * /* parent */ )
      : GaudiHistoAlg( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiHistoAlg> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiHistoAlg >::CommonBase( long /* storage_type */,
                                             const CLID & /* class_type */,
                                             ISvcLocator * /* svc */ )
      : GaudiHistoAlg( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiHistoAlg> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //----------------------------------------------------------------------------
    template <>
    HistoBase< GaudiHistoAlg >::HistoBase( const std::string &name, ISvcLocator *pSvcLocator )
      : CommonBase< GaudiHistoAlg >( name, pSvcLocator )
    {
      this->initRichHistoConstructor();
    }
    template <>
    HistoBase< GaudiHistoAlg >::HistoBase( const std::string &type,
                                           const std::string &name,
                                           const IInterface * parent )
      : CommonBase< GaudiHistoAlg >( type, name, parent )
    {}
    template <>
    HistoBase< GaudiHistoAlg >::HistoBase( long         storage_type,
                                           const CLID & class_type,
                                           ISvcLocator *svc )
      : CommonBase< GaudiHistoAlg >( storage_type, class_type, svc )
    {}
    //=============================================================================

    //=============================================================================
    // Implementation of CommonBase for GaudiTupleAlg
    //=============================================================================
    template <>
    CommonBase< GaudiTupleAlg >::CommonBase( const std::string &name, ISvcLocator *pSvcLocator )
      : GaudiTupleAlg( name, pSvcLocator )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiTupleAlg >::CommonBase( const std::string & /* type */,
                                             const std::string & /* name */,
                                             const IInterface * /* parent */ )
      : GaudiTupleAlg( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTupleAlg> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiTupleAlg >::CommonBase( long /* storage_type */,
                                             const CLID & /* class_type */,
                                             ISvcLocator * /* svc */ )
      : GaudiTupleAlg( "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTupleAlg> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //----------------------------------------------------------------------------
    template <>
    HistoBase< GaudiTupleAlg >::HistoBase( const std::string &name, ISvcLocator *pSvcLocator )
      : CommonBase< GaudiTupleAlg >( name, pSvcLocator )
    {
      this->initRichHistoConstructor();
      this->initRichTupleConstructor();
    }
    template <>
    HistoBase< GaudiTupleAlg >::HistoBase( const std::string &type,
                                           const std::string &name,
                                           const IInterface * parent )
      : CommonBase< GaudiTupleAlg >( type, name, parent )
    {}
    template <>
    HistoBase< GaudiTupleAlg >::HistoBase( long         storage_type,
                                           const CLID & class_type,
                                           ISvcLocator *svc )
      : CommonBase< GaudiTupleAlg >( storage_type, class_type, svc )
    {}
    //=============================================================================

    //=============================================================================
    // Implementation of CommonBase for GaudiTool
    //=============================================================================
    template <>
    CommonBase< GaudiTool >::CommonBase( const std::string &type,
                                         const std::string &name,
                                         const IInterface * parent )
      : GaudiTool( type, name, parent )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiTool >::CommonBase( const std::string & /* name */,
                                         ISvcLocator * /* pSvcLocator */ )
      : GaudiTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiTool >::CommonBase( long /* storage_type */,
                                         const CLID & /* class_type */,
                                         ISvcLocator * /* svc */ )
      : GaudiTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //=============================================================================

    //=============================================================================
    // Implementation of CommonBase for GaudiHistoTool
    //=============================================================================
    template <>
    CommonBase< GaudiHistoTool >::CommonBase( const std::string &type,
                                              const std::string &name,
                                              const IInterface * parent )
      : GaudiHistoTool( type, name, parent )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiHistoTool >::CommonBase( const std::string & /* name */,
                                              ISvcLocator * /* pSvcLocator */ )
      : GaudiHistoTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiHistoTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiHistoTool >::CommonBase( long /* storage_type */,
                                              const CLID & /* class_type */,
                                              ISvcLocator * /* svc */ )
      : GaudiHistoTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiHistoTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //-----------------------------------------------------------------------------
    template <>
    HistoBase< GaudiHistoTool >::HistoBase( const std::string &type,
                                            const std::string &name,
                                            const IInterface * parent )
      : CommonBase< GaudiHistoTool >( type, name, parent )
    {
      this->initRichHistoConstructor();
    }
    template <>
    HistoBase< GaudiHistoTool >::HistoBase( const std::string &name, ISvcLocator *pSvcLocator )
      : CommonBase< GaudiHistoTool >( name, pSvcLocator )
    {}
    template <>
    HistoBase< GaudiHistoTool >::HistoBase( long         storage_type,
                                            const CLID & class_type,
                                            ISvcLocator *svc )
      : CommonBase< GaudiHistoTool >( storage_type, class_type, svc )
    {}
    //=============================================================================

    //=============================================================================
    // Implementation of RichCommonBase for GaudiTupleTool
    //=============================================================================
    template <>
    CommonBase< GaudiTupleTool >::CommonBase( const std::string &type,
                                              const std::string &name,
                                              const IInterface * parent )
      : GaudiTupleTool( type, name, parent )
    {
      this->initRichCommonConstructor();
    }
    template <>
    CommonBase< GaudiTupleTool >::CommonBase( const std::string & /* name */,
                                              ISvcLocator * /* pSvcLocator */ )
      : GaudiTupleTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTupleTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< GaudiTupleTool >::CommonBase( long /* storage_type */,
                                              const CLID & /* class_type */,
                                              ISvcLocator * /* svc */ )
      : GaudiTupleTool( "ERROR", "ERROR", nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<GaudiTupleTool> constructor", "CommonBase", StatusCode::FAILURE );
    }
    //-----------------------------------------------------------------------------
    template <>
    HistoBase< GaudiTupleTool >::HistoBase( const std::string &type,
                                            const std::string &name,
                                            const IInterface * parent )
      : CommonBase< GaudiTupleTool >( type, name, parent )
    {
      this->initRichHistoConstructor();
      this->initRichTupleConstructor();
    }
    template <>
    HistoBase< GaudiTupleTool >::HistoBase( const std::string &name, ISvcLocator *pSvcLocator )
      : CommonBase< GaudiTupleTool >( name, pSvcLocator )
    {}
    template <>
    HistoBase< GaudiTupleTool >::HistoBase( long         storage_type,
                                            const CLID & class_type,
                                            ISvcLocator *svc )
      : CommonBase< GaudiTupleTool >( storage_type, class_type, svc )
    {}
    //=============================================================================

    //=============================================================================
    // Implementation of RichCommonBase for Converter
    //=============================================================================
    template <>
    CommonBase< Converter_Imp >::CommonBase( const std::string & /* type */,
                                             const std::string & /* name */,
                                             const IInterface * /* parent */ )
      : Converter_Imp( 0, CLID_NULL, nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<Converter_Imp> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< Converter_Imp >::CommonBase( const std::string & /* name */,
                                             ISvcLocator * /* pSvcLocator */ )
      : Converter_Imp( 0, CLID_NULL, nullptr )
    {
      throw GaudiException(
        "Invalid CommonBase<Rich::Converter_Imp> constructor", "CommonBase", StatusCode::FAILURE );
    }
    template <>
    CommonBase< Converter_Imp >::CommonBase( long         storage_type,
                                             const CLID & class_type,
                                             ISvcLocator *svc )
      : Converter_Imp( storage_type, class_type, svc )
    {
      this->initRichCommonConstructor();
    }
    //=============================================================================

  } // namespace Future
} // namespace Rich
