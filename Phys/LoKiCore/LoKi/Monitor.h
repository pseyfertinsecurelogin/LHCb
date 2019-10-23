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
#ifndef LOKI_MONITOR_H
#define LOKI_MONITOR_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/HistoDef.h"
#include "GaudiKernel/Kernel.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiHistoID.h"
// ============================================================================
class IHistogramSvc;
class IAlgContextSvc;
class IStatSvc;
class GaudiTool;
class GaudiAlgorithm;
class StatEntity;
// ============================================================================
namespace LoKi {
  // ==========================================================================
  class Histo;
  class CounterDef;
  // ==========================================================================
  namespace Monitoring {
    // ========================================================================
    /** helper enumerator to indicate the mode for creation of counters:
     *
     *   - ContextSvc : the counter is retrieved from the corresponding
     *                  GauidiAlgorithm using Context Service
     *   - StatSvc    : the counter is retrieved from Stat Service
     *
     *  @see StatEntity
     *  @see Stat
     *  @see IAlgContextSvc
     *  @see GaudiAlgorithm
     *  @see GaudiCommon
     *  @see IStatSvc
     */
    enum Flag {
      ContextSvc = 0, //  local counter through IAlgContext -> GaudiAlgorithm  ,
      StatSvc,        // global counter through IStatSvc  ,
    };
    // ========================================================================
    /** get the (global) counter by name using IStatSvc
     *  @param IStatSvc
     *  @param ssvc service of statistics
     *  @param name the counter name
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( IStatSvc* csvc, const std::string& name );
    /** get the (local) counter by name using GaudiAlgorithm
     *  @param alg the algorithm
     *  @param name the counter name
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( GaudiAlgorithm* alg, const std::string& name );
    /** get the (local) counter by name using GaudiTool
     *  @param tool the tool
     *  @param name the counter name
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( GaudiTool* tool, const std::string& name );
    /** get the counter by name using IAlgContextSvc
     *  @param name the counter name
     *  @param csvc context service
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( const IAlgContextSvc* csvc, const std::string& name );
    /** get the counter by name using IStatSvc/ICounter or IAlgContextSvc
     *  @param flag  local/global flag
     *  @param group the counter grop
     *  @param name  the counter name
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( const Flag flag, const std::string& group, const std::string& name );
    /** get the counter by name using IStatSvc/ICounter or IAlgContextSvc
     *  @param flag  local/global flag
     *  @param name  the counter name
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( const Flag flag, const std::string& name );
    /** get the counter using helper class
     *  @return the counter
     */
    GAUDI_API
    StatEntity* getCounter( const LoKi::CounterDef& cnt );
    // ========================================================================
  } // namespace Monitoring
  // ==========================================================================
  /** @class Histo LoKi/Monitor.h
   *  Special holder to postpone  the histogram booking
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2015-01-14
   */
  class GAUDI_API Histo {
  public:
    // ========================================================================
    /// constructors for Histogram service
    Histo( std::string dir, const std::string& id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc = nullptr );
    Histo( std::string dir, const int id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc = nullptr );
    Histo( std::string dir, const GaudiAlg::ID& id, Gaudi::Histo1DDef hdef, IHistogramSvc* svc = nullptr );
    Histo( std::string path, Gaudi::Histo1DDef hdef, IHistogramSvc* svc = nullptr );
    /// constructors for Context service
    Histo( Gaudi::Histo1DDef hdef, const std::string& id, IAlgContextSvc* svc = nullptr );
    Histo( Gaudi::Histo1DDef hdef, const int id, IAlgContextSvc* svc = nullptr );
    Histo( Gaudi::Histo1DDef hdef, const GaudiAlg::ID& id, IAlgContextSvc* svc = nullptr );
    /// default constructor
    Histo();
    // ========================================================================
  public:
    // ========================================================================
    [[nodiscard]] const std::string&       path() const { return m_path; }
    [[nodiscard]] const GaudiAlg::ID&      id() const { return m_id; }
    [[nodiscard]] const Gaudi::Histo1DDef& hdef() const { return m_hdef; }
    [[nodiscard]] bool                     hcase() const { return m_case; }
    [[nodiscard]] bool                     valid() const { return m_valid; }
    // ========================================================================
    [[nodiscard]] IHistogramSvc*  histoSvc() const { return m_hsvc; }
    [[nodiscard]] IAlgContextSvc* contextSvc() const { return m_cntx; }
    // ========================================================================
  private:
    //
    std::string       m_path;
    GaudiAlg::ID      m_id;
    Gaudi::Histo1DDef m_hdef;
    bool              m_case  = true;
    bool              m_valid = true;
    IHistogramSvc*    m_hsvc  = nullptr;
    IAlgContextSvc*   m_cntx  = nullptr;
    // ========================================================================
  };
  // ==========================================================================
  /** @class CounterDef
   *  helper class to keep defintino of counter
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2015-01-14
   */
  class GAUDI_API CounterDef {
  public:
    // ========================================================================
    CounterDef( std::string group, std::string name, const LoKi::Monitoring::Flag flag );
    CounterDef( std::string name = "", const LoKi::Monitoring::Flag f = LoKi::Monitoring::ContextSvc );
    // ========================================================================
  public:
    // ========================================================================
    [[nodiscard]] const std::string&     group() const { return m_group; }
    [[nodiscard]] const std::string&     name() const { return m_name; }
    [[nodiscard]] LoKi::Monitoring::Flag flag() const { return m_flag; }
    // ========================================================================
    [[nodiscard]] bool valid() const { return !m_name.empty(); }
    // ========================================================================
  private:
    // ========================================================================
    std::string            m_group;
    std::string            m_name;
    LoKi::Monitoring::Flag m_flag;
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
namespace Gaudi::Utils {
  // ========================================================================
  GAUDI_API std::string toCpp( const LoKi::Histo& o );
  GAUDI_API std::string toCpp( const LoKi::CounterDef& o );
  GAUDI_API std::string toCpp( const GaudiAlg::ID& o );
  GAUDI_API std::string toCpp( const Gaudi::Histo1DDef& o );
  // ========================================================================
} // namespace Gaudi::Utils
// ============================================================================
#endif // LOKI_MONITOR_H
