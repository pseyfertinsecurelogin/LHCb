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
// STD & STD
// ============================================================================
#include <sstream>
// ============================================================================
// HistoStrings
// ============================================================================
#include "GaudiUtils/Histo2String.h"
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/HistoParam.h"
// ============================================================================
/** @file
 *  Implementation file for the template specialization of Param
 *  methods for the histograms
 *  @see class Param
 *  @see class TH1D
 *  @see class TH2D
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2009-10-22
 */
// ============================================================================
// ============================================================================
/*   Interface function converting 1d histogram to xml string
 *   @see class Param
 *   @see class TH1D
 *   @param  histo     (INPUT) histogram
 *   @param  name      (INPUT) XML-tag name
 *   @param  comment   (INPUT) the comment
 *   @param  precision (INPUT) not used
 *   @return XML-string
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl, Dmitry Golubkov
 *   @date 2009-12-06
 */
// ============================================================================
std::string DetDesc::Params::toXMLStr( const DetDesc::Params::Histo1D& histo, const std::string& name,
                                       const std::string& comment, int /* precision */ ) {
  std::ostringstream out;
  //
  out << "<param name=\"" << name << "\" type=\"Histo1D\""; // open TAG
  if ( !comment.empty() ) { out << " comment=\"" << comment << '"'; }
  out << '>';
  // stream the histogram
  ////Gaudi::Utils::toStream ( m_val , out ) ;
  Gaudi::Utils::toStream( histo, out );
  out << "</param>"; // close TAG
  return out.str();
}
// ============================================================================
/*   Interface function converting 1d histogram to a custom format string
 *   @see class Param
 *   @see class TH1D
 *   @param  histo     (INPUT) histogram
 *   @return string
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl, Dmitry Golubkov
 *   @date 2009-12-06
 */
std::string DetDesc::Params::toStr( const DetDesc::Params::Histo1D& histo ) { return Gaudi::Utils::toString( histo ); }
// ============================================================================
/*   Interface function converting 2d histogram to xml string
 *   @see class Param
 *   @see class TH1D
 *   @param  histo     (INPUT) histogram
 *   @param  name      (INPUT) XML-tag name
 *   @param  comment   (INPUT) the comment
 *   @param  precision (INPUT) not used
 *   @return XML-string
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl, Dmitry Golubkov
 *   @date 2009-12-06
 */
std::string DetDesc::Params::toXMLStr( const DetDesc::Params::Histo2D& histo, const std::string& name,
                                       const std::string& comment, int /* precision */ ) {
  std::ostringstream out;
  //
  out << "<param name=\"" << name << "\" type=\"Histo2D\""; // open TAG
  if ( !comment.empty() ) { out << " comment=\"" << comment << '"'; }
  out << '>';
  // stream the histogram
  Gaudi::Utils::toStream( histo, out );
  out << "</param>"; // close TAG
  return out.str();
}
// ============================================================================
/*   Interface function converting 2d histogram to a custom format string
 *   @see class Param
 *   @see class TH1D
 *   @param  histo     (INPUT) histogram
 *   @return string
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl, Dmitry Golubkov
 *   @date 2009-12-06
 */
std::string DetDesc::Params::toStr( const DetDesc::Params::Histo2D& histo ) { return Gaudi::Utils::toString( histo ); }
// ============================================================================
// The END
// ============================================================================
