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
#ifndef LOKI_TES_H
#define LOKI_TES_H 1
// ============================================================================
// Include files
// ============================================================================
#include <memory>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/ObjectContainerBase.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
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
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
namespace LoKi {
  namespace TES {
    // ========================================================================
    /** @class Get
     *  helper class to get some objects from TES
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2013-03-28
     */
    class GAUDI_API Get : public virtual LoKi::AuxFunBase {
    public:
      // =======================================================================
      /** constructor from TES location and root-in-tes flag
       *  @param algorithm    own algorithm/context
       *  @param location     TES-location
       *  @param useRootInTES RootInTES-flag
       */
      Get( const GaudiAlgorithm* algorithm, const std::string& location, const bool useRootInTES = true );
      /** constructor from TES location
       *  @param datasvc    data service
       *  @param location   TES-location
       */
      Get( const IDataProviderSvc* datasvc, const std::string& location );
      /// virtual destructor
      ~Get() override;
      // ======================================================================
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the algorithym
      const LoKi::Interface<GaudiAlgorithm>& algorithm() const { return m_algorithm; }
      /// get the algorithym
      const LoKi::Interface<IDataProviderSvc>& datasvc() const { return m_datasvc; }
      /// get algorithm name
      const std::string& algName() const;
      // ======================================================================
    public:
      // ======================================================================
      /// path in TES
      const std::string& location() const { return m_location; }
      /// path in TES
      const std::string& path() const { return location(); }
      /// use root-in-tes?
      bool useRootInTES() const { return m_algorithm ? m_useRootInTES : false; }
      // ======================================================================
    protected:
      // ======================================================================
      void setLocation( const std::string& value ) { m_location = value; }
      // ======================================================================
    public:
      // ======================================================================
      /// notify that we need the context algorithm
      static bool context_algo() { return true; }
      /// notify that we need the context  service
      static bool context_evtsvc() { return true; }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual TES location
      std::string m_location; // the actual TES location
      /// "Root-in-TES" flag
      bool m_useRootInTES; // "Root-in-TES" flag
      /// the algorithm to be used
      mutable LoKi::Interface<GaudiAlgorithm> m_algorithm; // the algorithm
      /// the service   to be used
      mutable LoKi::Interface<IDataProviderSvc> m_datasvc; // the service
      // ======================================================================
    };
    // ========================================================================
    /** @class DataHandle
     *  @author Sascha Stahl
     */
    template <class TYPE>
    class GAUDI_API DataHandle : public virtual LoKi::AuxFunBase {
    public:
      // ======================================================================
      /// constructor from the context and TES location
      DataHandle( const GaudiAlgorithm* algorithm, const std::string& location )
          : LoKi::AuxFunBase( std::tie( algorithm, location ) )
          , m_location( location )
          , m_algorithm( algorithm )
          , m_datahandle( this->location(), this->algorithm().get() ) {}
      /// copy constructor
      DataHandle( const DataHandle& right )
          : LoKi::AuxFunBase( right )
          , m_location( right.location() )
          , m_algorithm( right.algorithm() )
          , m_datahandle( right.location(), right.algorithm().get() ) {}
      /// default constructor is disabled
      DataHandle() = delete;
      // ======================================================================
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " GET("
                 << "'" << this->location() << "')";
      }
      // ======================================================================
      const LoKi::Interface<GaudiAlgorithm>& algorithm() const { return m_algorithm; }
      const std::string&                     location() const { return m_location; }
      // ======================================================================
      decltype( auto ) get() const { return m_datahandle.get(); }
      // ======================================================================
    protected:
      // =======================================================================
      bool exist() const { return m_datahandle.exist(); }
      // ======================================================================
    public:
      // ======================================================================
      /// notify that we need he context algorithm
      static bool context_algo() { return true; }
      // ======================================================================
    private:
      // ======================================================================
      std::string                     m_location;
      LoKi::Interface<GaudiAlgorithm> m_algorithm;
      DataObjectReadHandle<TYPE>      m_datahandle;
      // =====================================================================
    };
    // ========================================================================
    /** @class Exists
     *  Simple checker of existence of object in TES
     *  @see LoKi::Cuts::EXISTS
     *  @see GaudiCommon<TYPE>::exists
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    class GAUDI_API Exists : public LoKi::Functor<void, bool>, public LoKi::TES::Get {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       */
      Exists( const GaudiAlgorithm* algorithm, const std::string& location, const bool useRootInTes = true );
      /// constructor from TES location
      Exists( const IDataProviderSvc* datasvc, const std::string& location );
      /// MANDATORY: clone method ("virtual constructor")
      Exists* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( /* argument v */ ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Contains
     *  Simple checker of existence & size of Container  in TES
     *  @see LoKi::Cuts::CONTAINS
     *  @see GaudiCommon<TYPE>::exists
     *  @see GaudiCommon<TYPE>::get
     *  @see ObjectContainerBase
     *  @see ObjectContainerBase::numberOfOjbects
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    class GAUDI_API Contains : public LoKi::Functor<void, double>, public LoKi::TES::Get {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       *  @see GaudiCommon<TYPE>::get
       */
      explicit Contains( const GaudiAlgorithm* algorithm, const std::string& location, const bool useRootInTes = true );
      /** constructor from TES location
       */
      explicit Contains( const IDataProviderSvc* datasvc, const std::string& location );
      /// MANDATORY: clone method ("virtual constructor")
      Contains* clone() const override;
      /** MANDATORY: the only one essential method
       *  @return number of elements in container, -1 for non-existing container
       */
      double operator()() const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Size
     *  <code>DataHandle</code>-based functor
     *  to check the size of data at certain TES-location
     *  - for non-existent data exeption is thrown
     *  - for data inherited from <code>ObjectContainerBase</code>
     *       <code>ObjectContainerBase::numberObjects</code> is returned
     *  - for data inhetited  from <code>AnyDataWrapperBase</code>
     *       <code>anydata->size().value_or   ( -1  )</code> is  returned
     *  - for unknown data type <code>-1</code> is returned
     *  @see LoKi::Cuts::SIZE
     *  @see LoKi::Cuts::CONTAINS
     *  @see LoKi::TES::DataHandle
     *  @see DataObjectReadHandle
     *  @see ObjectContainerBase::numberObjects
     *  @see AnyDataWrapperBase
     *  @author Sascha Stahl
     */
    class GAUDI_API Size : public LoKi::Functor<void, double>, public LoKi::TES::DataHandle<DataObject> {
    public:
      // ======================================================================
      Size( const GaudiAlgorithm* algortihm, const std::string& location );
      Size*         clone() const override;
      double        operator()() const override;
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class HasData
     *  <code>DataHandle</code>-based functor
     *  to check the existence of data at certain TES-location
     *  @see LoKi::Cuts::HASDATA
     *  @see LoKi::Cuts::EXISTS
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    class GAUDI_API HasData : public LoKi::Functor<void, bool>, public LoKi::TES::DataHandle<DataObject> {
    public:
      // ======================================================================
      HasData( const GaudiAlgorithm* algortihm, const std::string& location );
      HasData*      clone() const override;
      bool          operator()() const override;
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class HrcSumAdc
     *  Simple query to sum contents of Herschel Digits for a station
     *  @author Dan JOHNSON  daniel.johnson@cern.ch
     *  @date 2016-05-17
     */
    class GAUDI_API HrcSumAdc : public LoKi::Functor<void, double>, public LoKi::TES::Get {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       *  @see GaudiCommon<TYPE>::get
       */
      HrcSumAdc( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& stationName,
                 const bool useRootInTes = true );
      /// MANDATORY: clone method ("virtual constructor")
      HrcSumAdc* clone() const override;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      double operator()( /* argument v */ ) const override;
      /// OPTIONAL: nice printout
      std::ostream&      fillStream( std::ostream& s ) const override;
      const std::string& stationName() const { return m_stationName; }
      // ======================================================================
    private:
      // ======================================================================
      /// the station name
      std::string m_stationName; // the Herschel station name
      // ======================================================================
    };

    // ========================================================================
    /** @class Counter
     *  Simple accessor for counters in TES
     *  @see Gaudi::Numbers
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    class GAUDI_API Counter : public LoKi::TES::Contains {
    public:
      // ======================================================================
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
               const double bad, const bool useRootInTes = true );
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter );
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
               const double bad );
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter );
      /// MANDATORY: clone method ("virtual constructor")
      Counter* clone() const override;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      double operator()() const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the counter name
      const std::string& counter() const { return m_counter; }
      double             bad() const { return m_bad; }
      // ======================================================================
    private:
      // ======================================================================
      /// the counter name
      std::string m_counter; // the counter name
      /// the 'bad'-value
      double m_bad; // the 'bad'-value
      // ======================================================================
    };
    // ========================================================================
    // Helper class needed by Stat.
    class StatEntityGetter;
    // ========================================================================
    /** @class Stat
     *  Simple accessor for counters in TES
     *  @see Gaudi::Counters
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    class GAUDI_API Stat : public LoKi::TES::Counter {
    public:
      // ======================================================================
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
            const std::string& function, const double bad, const bool useRootInTes = true );
      Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
            const StatEntityGetter& function, const double bad, const bool useRootInTes = true );
      Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
            const std::string& function );
      Stat( const GaudiAlgorithm* algorithm, const std::string& location, const std::string& counter,
            const StatEntityGetter& function );
      // ======================================================================
      Stat( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
            const std::string& function, const double bad );
      Stat( const IDataProviderSvc* datasvc, const std::string& location, const std::string& counter,
            const std::string& function );
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Stat* clone() const override;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      double operator()() const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// Helper object used to extract information from the StatEntity object.
      std::shared_ptr<const StatEntityGetter> m_getter;
      // ======================================================================
    };
    // ========================================================================
    template <class TYPE>
    inline const TYPE* get_( const LoKi::TES::Get& obj ) {
      // ======================================================================
      if ( obj.algorithm() ) {
        return obj.algorithm()->getIfExists<TYPE>( obj.location(), obj.useRootInTES() );
      } else if ( obj.datasvc() ) {
        return SmartDataPtr<TYPE>( obj.datasvc(), obj.location() );
      }
      return nullptr;
      // ======================================================================
    }
    // ========================================================================
    template <class TYPE>
    inline bool exists_( const LoKi::TES::Get& obj ) {
      // ======================================================================
      if ( obj.algorithm() ) {
        return obj.algorithm()->exist<TYPE>( obj.location(), obj.useRootInTES() );
      } else if ( obj.datasvc() ) {
        SmartDataPtr<TYPE> o( obj.datasvc(), obj.location() );
        return !( !o );
      }
      return false;
      // ======================================================================
    }
    // ========================================================================
  } // namespace TES
} // namespace LoKi
// ============================================================================
namespace LoKi {
  namespace Cuts {
    // ========================================================================
    /** @typedef CONTAINS
     *  Trivial checker/function for existence & size of Container in TES
     *
     *  @code
     *
     *    400 > CONTAINS ( "/Raw/Spd/Digis")
     *
     *  @endcode
     *
     *  @see LoKi::TES::Contains
     *  @see GaudiCommon<TYPE>::exists
     *  @see GaudiCommon<TYPE>::get
     *  @see ObjectContainerBase
     *  @see ObjectContainerBase::numberOfOjbects
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    using CONTAINS = LoKi::TES::Contains;
    // ========================================================================
    /** @typedef SIZE
     *  <code>DataHandle</code>-based functor
     *  to check the size of data at certain TES-location
     *  - for non-existent data exception is thrown
     *  - for data inherited from <code>ObjectContainerBase</code>
     *       <code>ObjectContainerBase::numberObjects</code> is returned
     *  - for data inhetited  from <code>AnyDataWrapperBase</code>
     *       <code>anydata->size().value_or   ( -1  )</code> is  returned
     *  - for unknown data type <code>-1</code> is returned
     *  @code
     *   0 < SIZE ( '/Event/PSIX/Phys/SelPsi2KForPsiX/Particles' )
     *  @endcode
     *  @see LoKi::TES::Size
     *  @see LoKi::TES::DataHandle
     *  @see DataObjectReadHandle
     *  @see ObjectContainerBase::numberObjects
     *  @see AnyDataWrapperBase
     *  @author Sascha Stahl
     */
    using SIZE = LoKi::TES::Size;
    // ========================================================================

    // /** @typedef HRCSUMADC
    //  *  Function to find Herschel station sum digits
    //  *
    //  *  @code
    //  *
    //  *    400 > HRCSUMADC ( "/Raw/HC/Sum", "B2" )
    //  *
    //  *  @endcode
    //  *
    //  *  @see LoKi::TES::HrcSumAdc
    //  *  @author Dan JOHNSON  daniel.johnson@cern.ch
    //  *  @date 2016-05-17
    //  */
    // typedef LoKi::TES::HrcSumAdc                                    HRCSUMADC ;

    // ========================================================================
    /** @typedef EXISTS
     *  Trivial checker/predicate for existence of object in TES
     *
     *  @code
     *
     *     EXISTS ("Phys/StdJpsi2MuMu/Particles")
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::EXISTS
     *  @see GaudiCommon<TYPE>::exists
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    using EXISTS = LoKi::TES::Exists;
    // ========================================================================
    /** @typedef HASDATA
     *  <code>DataHandle</code>-based functor
     *  to check the existence of data at certain TES-location
     *  @see LoKi::TES::HasData
     *  @see LoKi::Cuts::HASDATA
     *  @see LoKi::Cuts::EXISTS
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     */
    using HASDATA = LoKi::TES::HasData;
    // ========================================================================
    /** @typedef COUNTER
     *  Trivial accessor to the counter in TES
     *
     *  @code
     *
     *     COUNTER ("Phys/MyCounters" , "nSPD" , -1 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::COUNTER
     *  @see Gaudi::Numbers
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    using COUNTER = LoKi::TES::Counter;
    // ========================================================================
    /** @typedef STAT
     *  Trivial accessor to the counter in TES
     *
     *  @code
     *
     *     STAT ("Phys/MyCounters" , "nXX"  , "mean" , -1 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::STAT
     *  @see Gaudi::Counters
     *  @see Gaudi::Counter
     *  @see StatEntity
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    using STAT = LoKi::TES::Stat;
    // ========================================================================
  } // namespace Cuts
} // namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_TES_H
