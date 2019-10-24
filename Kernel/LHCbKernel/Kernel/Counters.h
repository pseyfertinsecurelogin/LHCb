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
#ifndef LHCBKERNEL_COUNTERS_H
#define LHCBKERNEL_COUNTERS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <functional>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/VectorMap.h"
// ============================================================================
/** equality for StatEntity
 *  @todo move equality for StatEntoti obejct sto GaudiKernel
 */
inline bool operator==( const StatEntity& i, const StatEntity& j ) {
  return &i == &j ? true : i < j ? false : j < i ? false : true;
}
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  /** @class Counter
   *  Simple persistifiable counter in Transient Store
   *  @see StatEntity
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2010-11-02
   */
  class GAUDI_API Counter : public DataObject {
  public:
    // ========================================================================
    /// constructor from counter
    Counter( StatEntity cnt = StatEntity(), std::string desc = "" );
    // ========================================================================
  public: // get the description
    // ========================================================================
    /// the the description of the counter
    [[nodiscard]] const std::string& description() const { return m_description; }
    // ========================================================================
  public:
    // ========================================================================
    // explicit access to the acual counter
    StatEntity& counter() { return m_counter; }
    // explicit access to the acual counter (const-version)
    [[nodiscard]] const StatEntity& counter() const { return m_counter; }
    // ========================================================================
  public:
    // ========================================================================
    /// implicit cast to the actual counter
    operator StatEntity&() { return counter(); }
    /// implicit cast to the actual counter (const-version)
    operator const StatEntity&() const { return counter(); }
    // ========================================================================
  public:
    // ========================================================================
    /// set new counter
    void setCounter( const StatEntity& value ) { m_counter = value; }
    /// set new name (alias)
    void setDescription( const std::string& value ) { m_description = value; }
    // ========================================================================
  public:
    // ========================================================================
    /// printout  to std::ostream
    std::ostream& fillStream( std::ostream& o ) const override;
    // ========================================================================
  public: //                               Gaudi technicalities for DataObject
    // ========================================================================
    /// retrieve the unique object identifier  (virtual)
    [[nodiscard]] const CLID& clID() const override;
    /// retrieve the unique object identifier  (static)
    static const CLID& classID();
    // ========================================================================
  private:
    // ========================================================================
    /// the actual counter
    StatEntity m_counter; //           the actual counter
    /// the counter description
    std::string m_description; // the counter name/description
    // ========================================================================
  };
  // ==========================================================================
  /** @class Counters
   *  Simple persisifiable map of counters in TES
   *  @see StatEntity
   *  @see VectorMap
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2010-11-02
   */
  class GAUDI_API Counters : public DataObject {
  public:
    // ========================================================================
    /// define the actual mapping type
    typedef GaudiUtils::VectorMap<std::string, StatEntity> Map;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from counter
    Counters( Map m = Map() );
    // ========================================================================
  public: // the counter-map
    // ========================================================================
    // explicit access to the acual counter
    Map& counters() { return m_counters; }
    // explicit access to the acual counter (const-version)
    [[nodiscard]] const Map& counters() const { return m_counters; }
    // ========================================================================
  public:
    // ========================================================================
    /// implicit cast to the actual counters
    operator Map&() { return counters(); }
    /// implicit cast to the actual counter (const-version)
    operator const Map&() const { return counters(); }
    // ========================================================================
  public:
    // ========================================================================
    /// set new counter
    void setCounters( const Map& value ) { m_counters = value; }
    // ========================================================================
  public:
    // ========================================================================
    /// get the number of counters
    [[nodiscard]] std::size_t size() const { return m_counters.size(); }
    // ========================================================================
  public:
    // ========================================================================
    /// printout  to std::ostream
    std::ostream& fillStream( std::ostream& o ) const override;
    // ========================================================================
  public: //                               Gaudi technicalities for DataObject
    // ========================================================================
    /// retrieve the unique object identifier  (virtual)
    [[nodiscard]] const CLID& clID() const override;
    /// retrieve the unique object identifier  (static)
    static const CLID& classID();
    // ========================================================================
  private:
    // ========================================================================
    /// the actual counter map
    Map m_counters; //           the actual counter
    // ========================================================================
  };
  // ==========================================================================
  /** @class Numbers
   *  Simple persisifiable map of simple "counters" in TES
   *  @see StatEntity
   *  @see VectorMap
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2010-11-02
   */
  class GAUDI_API Numbers : public DataObject {
  public:
    // ========================================================================
    /// define the actual mapping type
    typedef GaudiUtils::VectorMap<std::string, double> Map;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from counter
    Numbers( Map m = Map() );
    // ========================================================================
  public: // the counter-map
    // ========================================================================
    // explicit access to the acual counter
    Map& counters() { return m_counters; }
    // explicit access to the acual counter (const-version)
    [[nodiscard]] const Map& counters() const { return m_counters; }
    // ========================================================================
    // explicit access to the acual counter
    Map& numbers() { return counters(); }
    // explicit access to the acual counter (const-version)
    [[nodiscard]] const Map& numbers() const { return counters(); }
    // ========================================================================
  public:
    // ========================================================================
    /// implicit cast to the actual counters
    operator Map&() { return counters(); }
    /// implicit cast to the actual counter (const-version)
    operator const Map&() const { return counters(); }
    // ========================================================================
  public:
    // ========================================================================
    /// set new counter
    void setCounters( const Map& value ) { m_counters = value; }
    /// set new counter
    void setNumbers( const Map& value ) { setCounters( value ); }
    // ========================================================================
  public:
    // ========================================================================
    /// get the number of counters
    [[nodiscard]] std::size_t size() const { return m_counters.size(); }
    // ========================================================================
  public:
    // ========================================================================
    /// printout  to std::ostream
    std::ostream& fillStream( std::ostream& o ) const override;
    // ========================================================================
  public: //                               Gaudi technicalities for DataObject
    // ========================================================================
    /// retrieve the unique object identifier  (virtual)
    [[nodiscard]] const CLID& clID() const override;
    /// retrieve the unique object identifier  (static)
    static const CLID& classID();
    // ========================================================================
  private:
    // ========================================================================
    /// the actual counter map
    Map m_counters; //           the actual counter
    // ========================================================================
  };
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
// The END
// ============================================================================
#endif // LHCBKERNEL_COUNTERS_H
