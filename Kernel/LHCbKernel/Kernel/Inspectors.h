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
#ifndef KERNEL_INSPECTORS_H
#define KERNEL_INSPECTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD &STL
// ============================================================================
#include <iosfwd>
#include <string>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// forward decalrations
// ============================================================================
class IInterface;
class IProperty;
// ============================================================================
namespace Gaudi::Utils {
  // ========================================================================
  /// the actual type for the vector of properties
  using Properties = std::vector<const ::Property*>;
  /// the actual type for the vector of property names
  using Names = std::vector<std::string>;
  // ========================================================================
  /** simple function to extract from the given component
   *  the list of properties, specified by name
   *
   *  @code
   *
   *  const Gaudi::Utils::Names& names = ... ;
   *
   *  const IInterface* component = ... ;
   *
   *  Gaudi::Utils::Properties props =
   *    Gaudi::Utils::properties ( component , names ) ;
   *
   *  for( Gaudi::Utils::Properties::const_iterator ip = props.begin() ;
   *       props.end () != ip ; ++ip )
   *   {
   *      const Property* p = *ip ;
   *      if ( 0 == p ) { continue ; }
   *      // print property:
   *      std::cout << (*p) << std::endl ;
   *   }
   *
   *  @endcode
   *
   *  @see Gaudi::Utils::property
   *  @param cmp the component
   *  @param name list of property names
   *  @retutn the list of properties
   *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
   *  @date 2008-08-02
   */
  Properties properties( const IInterface* cmp, const Names& names );
  // ========================================================================
  /** simple function to extract from the given component
   *  the list of properties, specified by name
   *
   *  @code
   *
   *  const Gaudi::Utils::Names& names = ... ;
   *
   *  const IInterface* component = ... ;
   *
   *  Gaudi::Utils::Properties props =
   *    Gaudi::Utils::properties ( component , names ) ;
   *
   *  for( Gaudi::Utils::Properties::const_iterator ip = props.begin() ;
   *       props.end () != ip ; ++ip )
   *   {
   *      const Property* p = *ip ;
   *      if ( 0 == p ) { continue ; }
   *      // print property:
   *      std::cout << (*p) << std::endl ;
   *   }
   *
   *  @endcode
   *
   *  @see Gaudi::Utils::property
   *  @param cmp the component
   *  @param name list of property names
   *  @param output (OUTPUT) the list of properties
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-08-02
   */
  void properties( const IInterface* cmp, const Names& names, Properties& output );
  // ========================================================================
} // namespace Gaudi::Utils
// ============================================================================
namespace LHCb {
  // ==========================================================================
  /** @class Inspector
   *  Simple inspector of algorithm properties
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-08-02
   */
  class Inspector {
  public:
    // ========================================================================
    /** constructor the list of properties
     *  @param props   list of properties to be inspected
     */
    Inspector( Gaudi::Utils::Names props = Gaudi::Utils::Names() );
    /** constructor the list of properties
     *  @param props   list of properties to be inspected
     *  @param members list of structural properties
     */
    Inspector( Gaudi::Utils::Names props, Gaudi::Utils::Names members );
    // ========================================================================
  public:
    // ========================================================================
    /** inspect the component
     *
     *    - 500   NULL pointer is used
     *    - 501   Gaudi::svcLocator points to NULL
     *    - 502   Unable to extract IAlgManager
     *    - 503   IAlgorithm points to NULL
     *    - 510+N Error code from IAlgManager::getAlgorithm
     *
     *  @param component the component to be inspected
     *  @param stream the output stream
     *  @param level the recursion level
     *  @return status code
     */
    StatusCode inspect( const IInterface* component, std::ostream& stream, const size_t level = 0 ) const;
    // ========================================================================
    /** inspect the component
     *  @param component the component to be inspected
     *  @param level the recursion level
     *  @return the inspection result
     */
    std::string inspect( const IInterface* component, const size_t level = 0 ) const;
    // ========================================================================
  public:
    // ========================================================================
    /** set new algorithm manager (optiojally).
     *
     *  Status codes:
     *
     *    - 504 NULL pointer is used
     *    - 505 IAlgManage ris invalid after the assignement
     *
     *  @param mgr new algorithm manager
     *  @return status code
     */
    StatusCode setAlgManager( const IInterface* mgr );
    // ========================================================================
  private:
    // ========================================================================
    /// structural properties
    std::vector<std::string> m_members;
    /// properties toe binspected
    std::vector<std::string> m_names;
    // ========================================================================
    // the algorithm manager
    SmartIF<IAlgManager> m_algMgr;
    // ========================================================================
  };
  // ==========================================================================
} // namespace LHCb
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_INSPECTORS_H
