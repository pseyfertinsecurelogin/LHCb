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
#ifndef LOKI_PARAM_H
#define LOKI_PARAM_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// forward declarations
// ============================================================================
/* @file LoKi/Param.h
 *
 * This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 * The package has been designed with the kind help from
 * Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 * contributions and advices from G.Raven, J.van Tilburg,
 * A.Golutvin, P.Koppenburg have been used in the design.
 *
 * @date 2014-02-02
 * @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *
 */
namespace LoKi {
  // ==========================================================================
  /** @class Param LoKi/Param.h
   *  simple representation of "parameter"
   *  @author Vanya Belyaev
   *  @date   2014-02-02
   *  @thanks Alexander MAZUROV for regex parsing code
   */
  class GAUDI_API Param final {
  public:
    // ========================================================================
    /** Constructor from full specialization
     *  format: 'property[key]/algorithm'
     *  - property name
     *  - key in property, if property is map-property
     *  - the (regex)-pattern for the algorithm name
     */
    Param( const std::string& property = "" );
    //
    // ========================================================================
  public:
    // ========================================================================
    [[nodiscard]] const std::string& property() const { return m_property; }
    [[nodiscard]] const std::string& key() const { return m_key; }
    [[nodiscard]] const std::string& algorithm() const { return m_algorithm; }
    // ========================================================================
  public:
    // ========================================================================
    /// printout
    std::ostream& fillStream( std::ostream& s ) const;
    // ========================================================================
  private:
    // ========================================================================
    /// the property name
    std::string m_property;
    /// the key name if property is a map
    std::string m_key;
    /// the algorithm pattern (if relevant)
    std::string m_algorithm;
    // ========================================================================
  };
  // ==========================================================================
  /** output operator of function objects to std::ostream
   *  @param stream reference to the stream
   *  @param obj object to be printed
   *  @return reference to the stream
   */
  inline std::ostream& operator<<( std::ostream& stream, const Param& obj ) { return obj.fillStream( stream ); }
  // ==========================================================================
} // namespace LoKi
// ============================================================================
namespace Gaudi::Utils {
  // ========================================================================
  GAUDI_API std::string toCpp( const LoKi::Param& o );
  // ========================================================================
} // namespace Gaudi::Utils
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_PARAM_H
