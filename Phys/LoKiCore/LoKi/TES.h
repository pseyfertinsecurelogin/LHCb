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
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
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
namespace LoKi
{
  // ==========================================================================
  namespace TES
  {
    // ========================================================================
    /** @class Get
     *  helper class to get some objects from TES
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2013-03-28
     */
    class GAUDI_API Get : public virtual LoKi::AuxFunBase
    {
    public:
      // =======================================================================
      /** constructor from TES location and root-in-tes flag
       *  @param location     TES-location
       *  @param useRootInTES RootInTES-flag
       */
      explicit Get ( const std::string& location            ,
                     const bool         useRootInTES = true ) ;
      /// virtual destructor
      virtual ~Get() ;
      // ======================================================================
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      const LoKi::Interface<GaudiAlgorithm>&   algorithm () const
      { return m_algorithm ; }
      const LoKi::Interface<IDataProviderSvc>& service   () const
      { return m_datasvc   ; }
      const LoKi::Interface<IDataProviderSvc>& dataSvc   () const
      { return m_datasvc   ; }
      const std::string& location     () const { return m_location     ; }
      bool               useRootInTES () const { return m_useRootInTES ; }
      /// get algorithm name
      const std::string& algName      () const ;
      // ======================================================================
    protected:
      // ======================================================================
      void setLocation ( const std::string& value ) { m_location = value ; }
      /// acquire algorithm or service 
      void getAlgSvc   () const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual TES location
      std::string m_location     ; // the actual TES location
      /// "Root-in-TES" flag
      bool        m_useRootInTES ; // "Root-in-TES" flag
      /// the algorithm to be used
      mutable LoKi::Interface<GaudiAlgorithm>   m_algorithm ;  // the algorithm
      /// the service   to be used
      mutable LoKi::Interface<IDataProviderSvc> m_datasvc   ;  // the service
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Exists
     *  Simple checker of existence of object in TES
     *  @see LoKi::Cuts::EXISTS
     *  @see GaudiCommon<TYPE>::exists
     *  The concept belong to Gerhard The Great Raven
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    class GAUDI_API Exists
      : public LoKi::Functor<void,bool>
      , public LoKi::TES::Get
    {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       */
      explicit Exists ( const std::string& location            ,
                        const bool         useRootInTes = true ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Exists* clone() const ;
      /// MANDATORY: the only one essential method
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Contains
     *  Simple checker of existence & size of Container  in TES
     *  @see LoKi::Cuts::CONTAINS
     *  @see GaudiCommon<TYPE>::exists
     *  @see GaudiCommon<TYPE>::get
     *  @see ObjectContainerBase
     *  @see ObjectContainerBase::numberOfOjbects
     *  The concept belong to Gerhard The Great Raven
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    class GAUDI_API Contains
      : public LoKi::Functor<void,double>
      , public LoKi::TES::Get
    {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       *  @see GaudiCommon<TYPE>::get
       */
      explicit Contains ( const std::string& location            ,
                          const bool         useRootInTes = true ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Contains* clone() const ;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class HrcSumAdc
     *  Simple query to sum contents of Herschel Digits for a station
     *  @author Dan JOHNSON  daniel.johnson@cern.ch
     *  @date 2016-05-17
     */
    class GAUDI_API HrcSumAdc
      : public LoKi::Functor<void,double>
      , public LoKi::TES::Get
    {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag
       *  @see GaudiCommon<TYPE>::exists
       *  @see GaudiCommon<TYPE>::get
       */
      HrcSumAdc ( const std::string& location              ,
                  const std::string& stationName           ,
                  const bool         useRootInTes = true ) ;
      /// MANDATORY: clone method ("virtual constructor")
      HrcSumAdc* clone() const override ;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      result_type operator() ( /* argument v */ ) const override ;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override ;
      const std::string& stationName     () const { return m_stationName    ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the station name
      std::string m_stationName         ; // the Herschel station name
      // ======================================================================
    } ;
    // ========================================================================
    /** @class Counter
     *  Simple accessor for counters in TES
     *  @see Gaudi::Numbers
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    class GAUDI_API Counter : public LoKi::TES::Contains
    {
    public:
      // ======================================================================
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter ( const std::string& location              ,
                const std::string& counter               ,
                const double       bad                   ,
                const bool         useRootInTes = true   ) ;
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Counter ( const std::string& location              ,
                const std::string& counter               ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Counter* clone() const ;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the counter name
      const std::string& counter () const { return m_counter ; }
      double             bad     () const { return m_bad     ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the counter name
      std::string m_counter ;                               // the counter name
      /// the 'bad'-value
      double      m_bad     ;                                // the 'bad'-value
      // ======================================================================
    } ;
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
    class GAUDI_API Stat : public LoKi::TES::Counter
    {
    public:
      // ======================================================================
      /** constructor from TES location, counter name,
       *  bad value and root-intes flag
       */
      Stat ( const std::string&       location              ,
             const std::string&       counter               ,
             const std::string&       function              ,
             const double             bad                   ,
             const bool               useRootInTes = true   ) ;
      Stat ( const std::string&       location              ,
             const std::string&       counter               ,
             const StatEntityGetter&  function              ,
             const double             bad                   ,
             const bool               useRootInTes = true   ) ;
      Stat ( const std::string&       location              ,
             const std::string&       counter               ,
             const std::string&       function              ) ;
      Stat ( const std::string&       location              ,
             const std::string&       counter               ,
             const StatEntityGetter&  function              ) ;
      /// MANDATORY: virtual destructor
      virtual ~Stat () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Stat* clone() const ;
      /** MANDATORY: the only one essential method
       *  @return numebr of element in continer, -1 for non-existing container
       */
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// Helper object used to extract information from the StatEntity object.
      std::shared_ptr<StatEntityGetter> m_getter;
      // ======================================================================
    } ;
    // ========================================================================
    template <class TYPE>
    inline
    const TYPE*
    get_ ( const LoKi::TES::Get&  obj )
    {
      if      ( !(!obj.algorithm() ) )
      { return obj.algorithm()->getIfExists<TYPE> ( obj.location () , obj.useRootInTES () ) ; }
      else if ( !(!obj.service  () ) )
      { return SmartDataPtr<TYPE>                 ( obj.service  () , obj.location     () ) ; }
      //
      return 0 ;
    }
    // ========================================================================
    template <class TYPE>
    inline
    bool
    exists_ ( const LoKi::TES::Get&  obj )
    {
      if      ( !(!obj.algorithm () ) )
      { return obj.algorithm()->exist<TYPE> ( obj.location () , obj.useRootInTES () ) ; }
      else if ( !(!obj.service   () ) )
      {
        SmartDataPtr<TYPE> o                ( obj.service  () , obj.location     () ) ;
        return !(!o) ;
      }
      //
      return false  ;
    }
    // ========================================================================
  } //                                               end of namespace LoKi::TES
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Cuts
  {
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
    typedef LoKi::TES::Contains                                      CONTAINS ;
    // ========================================================================
    /** @typedef HRCSUMADC
     *  Function to find Herschel station sum digits 
     *
     *  @code
     *
     *    400 > HRCSUMADC ( "/Raw/HC/Sum", "B2" )
     *
     *  @endcode
     *
     *  @see LoKi::TES::HrcSumAdc
     *  @author Dan JOHNSON  daniel.johnson@cern.ch
     *  @date 2016-05-17
     */
    typedef LoKi::TES::HrcSumAdc                                    HRCSUMADC ;

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
     *  The concept belong to Gerhard The Great Raven
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    typedef LoKi::TES::Exists                                          EXISTS ;
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
    typedef LoKi::TES::Counter                                        COUNTER ;
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
    typedef LoKi::TES::Stat                                              STAT ;
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_TES_H
// ============================================================================
