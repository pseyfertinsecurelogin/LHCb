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
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/Fill.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiAlg/GaudiHistos.h"
#include "GaudiAlg/GaudiHistos.icpp"
#include "GaudiAlg/GetAlgs.h"
#include "GaudiAlg/IHistoTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HistoBook.h"
#include "LoKi/Monitor.h"
#include "LoKi/Report.h"
#include "LoKi/Services.h"
// ============================================================================
/** @file
 *  Implementation file for namespace LoKi::HistoBook
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-28
 */
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param svc pointer to Histogram Service
 *  @param path full path in Histogram Data Store
 *  @param hist histogram desctription
 *  @param histogram service
 *  @return booked histogram
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const std::string& path, const Gaudi::Histo1DDef& hist,
                                           IHistogramSvc* svc ) {
  // get the service from LoKi
  if ( !svc ) {
    const LoKi::Services& svcs = LoKi::Services::instance();
    svc                        = svcs.histoSvc();
  }
  //
  if ( !svc ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( path ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid IHistogramSvc" );
    return nullptr;
  }
  // check the existing histogram:
  AIDA::IHistogram1D* histo = nullptr;
  StatusCode          sc    = svc->retrieveObject( path, histo );
  if ( sc.isSuccess() && histo ) { return histo; } // RETURN
  // book new histogram:
  histo = Gaudi::Histos::book( svc, path, hist );
  if ( !histo ) {
    LoKi::Report::Error( "LoKi::HistoBook::book('" + path + "'," + Gaudi::Utils::toString( hist ) +
                         "): invalid AIDA::IHistogramID" );
    return nullptr;
  }
  //
  LoKi::Report::Print( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( path ) + "," +
                           Gaudi::Utils::toString( hist ) + ")" + " success using IHistogramSvc," + " THS path: '" +
                           Gaudi::Utils::Histos::path( Gaudi::Utils::Histos::toBase( histo ) ) + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return histo;
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param svc  pointer to Histogram Service
 *  @param dir  directory path in Histogram Data Store
 *  @param id   historgam identifier
 *  @param hist histogram desctription
 *  @param histogram service
 *  @return booked histogram
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const std::string& dir, const GaudiAlg::ID& id,
                                           const Gaudi::Histo1DDef& hist, IHistogramSvc* svc ) {
  // get the service form LoKi
  if ( !svc ) {
    const LoKi::Services& svcs = LoKi::Services::instance();
    svc                        = svcs.histoSvc();
  }
  //
  if ( !svc ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( dir ) + "," + Gaudi::Utils::toString( id ) +
                         "," + Gaudi::Utils::toString( hist ) + "): invalid IHistogramSvc" );
    return nullptr;
  }
  // check the existing histogram:
  AIDA::IHistogram1D* histo = nullptr;
  StatusCode          sc    = id.literal() ? svc->retrieveObject( dir, id.literalID(), histo )
                               : id.numeric() ? svc->retrieveObject( dir, id.numericID(), histo )
                                              : svc->retrieveObject( dir, id.idAsString(), histo );
  //
  if ( sc.isSuccess() && histo ) { return histo; } // RETURN
  // book new histogram:
  histo = id.literal() ? Gaudi::Histos::book( svc, dir, id.literalID(), hist )
                       : id.numeric() ? Gaudi::Histos::book( svc, dir, id.numericID(), hist )
                                      : Gaudi::Histos::book( svc, dir, id.idAsString(), hist );
  //
  if ( !histo ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( dir ) + "," + Gaudi::Utils::toString( id ) +
                         "," + Gaudi::Utils::toString( hist ) + "): invaild AIDA::IHistogramID" );
    return nullptr;
  }
  //
  LoKi::Report::Print( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( dir ) + "," + Gaudi::Utils::toString( id ) +
                           "," + Gaudi::Utils::toString( hist ) + ")" + " success using IHistogramSvc," +
                           " THS path: '" + Gaudi::Utils::Histos::path( Gaudi::Utils::Histos::toBase( histo ) ) + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return histo;
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param svc pointer to Histogram Service
 *  @param dir directory path in Histogram Data Store
 *  @param id  historgam identifier
 *  @param hist histogram desctription
 *  @param histogram service
 *  @return booked histogram
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const std::string& dir, const int id, const Gaudi::Histo1DDef& hist,
                                           IHistogramSvc* svc ) {
  return book( dir, GaudiAlg::ID( id ), hist, svc );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param svc pointer to Histogram Service
 *  @param dir directory path in Histogram Data Store
 *  @param id  historgam identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const std::string& dir, const std::string& id, const Gaudi::Histo1DDef& hist,
                                           IHistogramSvc* svc ) {
  return book( dir, GaudiAlg::ID( id ), hist, svc );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param alg  the algorithm, whcih owns the histogram
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const GaudiHistoAlg* alg, const std::string& id,
                                           const Gaudi::Histo1DDef& hist ) {
  return book( alg, GaudiAlg::ID( id ), hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param tool the tool, which owns the histogram
 *  @param dir  directory path in Histogram Data Store
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const GaudiHistoTool* tool, const std::string& id,
                                           const Gaudi::Histo1DDef& hist ) {
  return book( tool, GaudiAlg::ID( id ), hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param tool the tool, which owns the histogram
 *  @param dir  directory path in Histogram Data Store
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const IHistoTool* tool, const std::string& id,
                                           const Gaudi::Histo1DDef& hist ) {
  return book( tool, GaudiAlg::ID( id ), hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param alg  the algorithm, whcih owns the histogram
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const GaudiHistoAlg* alg, const GaudiAlg::ID& id,
                                           const Gaudi::Histo1DDef& hist ) {
  if ( !alg ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid GaudiHistoAlg" );
    return nullptr;
  }
  //
  if ( !alg->produceHistos() ) {
    LoKi::Report::Warning( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                           Gaudi::Utils::toString( hist ) + ")," + " histos are disabled for GaudiHistoAlg: '" +
                           alg->name() + "'" );
    return nullptr;
  }
  // book the histogram:
  AIDA::IHistogram1D* histo = alg->book( id, hist.title(), hist.lowEdge(), hist.highEdge(), hist.bins() );
  //
  if ( !histo ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid AIDA::IHistogram1D" );
    return nullptr;
  }
  //
  LoKi::Report::Print( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," + Gaudi::Utils::toString( hist ) +
                           ")" + " success using GaudiHistoAlg: '" + alg->name() + "'" + " THS path: '" +
                           Gaudi::Utils::Histos::path( Gaudi::Utils::Histos::toBase( histo ) ) + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return histo;
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param tool the tool, which owns the histogram
 *  @param dir  directory path in Histogram Data Store
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const GaudiHistoTool* tool, const GaudiAlg::ID& id,
                                           const Gaudi::Histo1DDef& hist ) {
  if ( !tool ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid GaudiHistoTool" );
    return nullptr;
  }
  //
  if ( !tool->produceHistos() ) {
    LoKi::Report::Warning( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                           Gaudi::Utils::toString( hist ) + ")," + " histos are disabled for GaudiHistoTool: '" +
                           tool->name() + "'" );
    return nullptr;
  }
  // book the histogram:
  AIDA::IHistogram1D* histo = tool->book( id, hist.title(), hist.lowEdge(), hist.highEdge(), hist.bins() );
  //
  if ( !histo ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid AIDA::IHistogramID" );
    return nullptr;
  }
  //
  LoKi::Report::Print( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," + Gaudi::Utils::toString( hist ) +
                           ")" + " success using GaudiHistoTool: " + tool->name() + "'" + " THS path: '" +
                           Gaudi::Utils::Histos::path( Gaudi::Utils::Histos::toBase( histo ) ) + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return histo;
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param tool the tool, which owns the histogram
 *  @param dir  directory path in Histogram Data Store
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const IHistoTool* tool, const GaudiAlg::ID& id,
                                           const Gaudi::Histo1DDef& hist ) {
  if ( !tool ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid IHistoTool" );
    return nullptr;
  }
  // book the histogram:
  AIDA::IHistogram1D* histo = tool->book( id, hist.title(), hist.lowEdge(), hist.highEdge(), hist.bins() );
  //
  if ( !histo ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid AIDA::IHistogramID" );
    return nullptr;
  }
  //
  LoKi::Report::Print( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," + Gaudi::Utils::toString( hist ) +
                           ")" + " success using IHistoTool: '" + tool->name() + "'" + " THS path: '" +
                           Gaudi::Utils::Histos::path( Gaudi::Utils::Histos::toBase( histo ) ) + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return histo;
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param svc the context service
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const IAlgContextSvc* svc, const GaudiAlg::ID& id,
                                           const Gaudi::Histo1DDef& hist ) {
  if ( !svc ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( id ) + "," +
                         Gaudi::Utils::toString( hist ) + "): invalid IAlgContextSvc" );
    return nullptr;
  }
  GaudiHistoAlg* alg = Gaudi::Utils::getHistoAlg( svc );
  return book( alg, id, hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @attention it looks for the appropriate Histo-algorithm within "context"
 *  @param svc the context service
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const Gaudi::Histo1DDef& hist, const GaudiAlg::ID& id,
                                           const IAlgContextSvc* svc ) {
  if ( !svc ) {
    const LoKi::Services& svcs = LoKi::Services::instance();
    svc                        = svcs.contextSvc();
  }
  //
  if ( !svc ) {
    LoKi::Report::Error( "LoKi::HistoBook::book(" + Gaudi::Utils::toString( hist ) + "," +
                         Gaudi::Utils::toString( id ) + "): invalid IAlgContextSvc" );
    return nullptr; // RETURN
  }
  return book( svc, id, hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @attention it looks for the appropriate Histo-algorithm within "context"
 *  @param svc the context service
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const Gaudi::Histo1DDef& hist, const std::string& id,
                                           const IAlgContextSvc* svc ) {
  return book( hist, GaudiAlg::ID( id ), svc );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @attention it looks for the appropriate Histo-algorithm within "context"
 *  @param svc the context service
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const Gaudi::Histo1DDef& hist, const int id, const IAlgContextSvc* svc ) {
  return book( hist, GaudiAlg::ID( id ), svc );
}
// ============================================================================
/** helper function to book 1D-histogram
 *  @param svc the context service
 *  @param id   histogram identifier
 *  @param hist histogram desctription
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const IAlgContextSvc* svc, const std::string& id,
                                           const Gaudi::Histo1DDef& hist ) {
  return book( svc, GaudiAlg::ID( id ), hist );
}
// ============================================================================
/*  helper function to book 1D-histogram
 *  @param hist histogram desctription
 *  @return booked histogram
 */
// ============================================================================
AIDA::IHistogram1D* LoKi::HistoBook::book( const LoKi::Histo& histo ) {
  //
  if ( histo.hcase() && histo.id().undefined() ) { return book( histo.path(), histo.hdef(), histo.histoSvc() ); }
  if ( histo.hcase() ) { return book( histo.path(), histo.id(), histo.hdef(), histo.histoSvc() ); }
  //
  return book( histo.hdef(), histo.id(), histo.contextSvc() );
}
// ============================================================================
// The END
// ============================================================================
