// $Id: TES.h 120745 2011-03-24 19:20:36Z ibelyaev $
// ============================================================================
#ifndef LOKI_TES_H 
#define LOKI_TES_H 1
// ============================================================================
// Include files
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
 *  By usage of this code one clearly states the disagreement 
 *  with the campain of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *                    $Revision: 120745 $
 *  Last modification $Date: 2011-03-24 20:20:36 +0100 (Thu, 24 Mar 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace TES
  {
    // ========================================================================
    /** @class Exists 
     *  Simple checker of existence of object in TES 
     *  @see LoKi::Cuts::EXISTS 
     *  @see GaudiCommon<TYPE>::exists 
     *  The concept belong to Gerhard The Great Raven 
     *  @author Vanya BELYAEV  Ivan.BElyaev@nikhef.nl
     *  @date 2010-02-13
     */
    class GAUDI_API Exists : public LoKi::Functor<void,bool>
    {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag 
       *  @see GaudiCommon<TYPE>::exists 
       */
      Exists ( const std::string& location            , 
               const bool         useRootInTes = true ) ;
      /// MANDATORY: virtual destructor 
      virtual ~Exists() ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Exists* clone() const ;
      /// MANDATORY: the only one essential method 
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get TES-location
      const  std::string& location     () const { return m_location     ; }
      inline bool         useRootInTes () const { return m_useRootInTes ; }
      /// get algorithm name 
      const std::string& algName  () const ;
      // ======================================================================
    protected:
      // ======================================================================
      void setLocation ( const std::string& value ) { m_location = value ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      Exists() ;                        // the default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// TES location to be tested 
      std::string                     m_location ; // TES location to be tested 
      /// Use 'RootInTes' ?
      bool                            m_useRootInTes ; //     Use 'RootInTes' ?
      // ======================================================================
    protected:
      // ======================================================================
      /// the algorithm to be used 
      mutable LoKi::Interface<GaudiAlgorithm> m_algorithm ;  // the algorithm 
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
    class GAUDI_API Contains : public LoKi::Functor<void,double>
    {
    public:
      // ======================================================================
      /** constructor from TES location & "rootInTes"-flag 
       *  @see GaudiCommon<TYPE>::exists 
       *  @see GaudiCommon<TYPE>::get 
       */
      Contains ( const std::string& location            , 
                 const bool         useRootInTes = true ) ;
      /// MANDATORY: virtual destructor 
      virtual ~Contains () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Contains* clone() const ;
      /** MANDATORY: the only one essential method 
       *  @return numebr of element in continer, -1 for non-existing container
       */
      virtual  result_type operator() ( /* argument v */ ) const ;
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get TES-location
      const  std::string& location     () const { return m_location     ; }
      inline bool         useRootInTes () const { return m_useRootInTes ; }
      /// get algorithm name 
      const std::string& algName  () const ;
      // ======================================================================
    protected:
      // ======================================================================
      void setLocation ( const std::string& value ) { m_location = value ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      Contains () ;                      // the default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// TES location to be tested 
      std::string                     m_location ; // TES location to be tested 
      /// Use 'RootInTes' ?
      bool                            m_useRootInTes ; //     Use 'RootInTes' ?
      // ======================================================================
    protected:
      // ======================================================================
      /// the algorithm to be used 
      mutable LoKi::Interface<GaudiAlgorithm> m_algorithm ; // the algorithm
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
    class GAUDI_API Counter : public Contains 
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
      /// MANDATORY: virtual destructor 
      virtual ~Counter () ;
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
      /// the default constructor is disabled 
      Counter () ;                       // the default constructor is disabled 
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
    /** @class Stat
     *  Simple accessor for counters in TES 
     *  @see Gaudi::Counters 
     *
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2011-02-12
     */
    class GAUDI_API Stat : public Counter 
    {
    public:
      // ======================================================================
      // the actual type of pointer to member function
      typedef double (StatEntity::*PMF)() const ;
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from TES location, counter name, 
       *  bad value and root-intes flag 
       */
      Stat ( const std::string& location              , 
             const std::string& counter               , 
             const std::string& function              , 
             const double       bad                   , 
             const bool         useRootInTes = true   ) ;
      Stat ( const std::string& location              , 
             const std::string& counter               , 
             PMF                function              , 
             const double       bad                   , 
             const bool         useRootInTes = true   ) ;
      Stat ( const std::string& location              , 
             const std::string& counter               , 
             const std::string& function              ) ;
      Stat ( const std::string& location              , 
             const std::string& counter               , 
             PMF                function              ) ;
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
      /// the default constructor is disabled 
      Stat () ;                          // the default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// the actual function 
      PMF m_fun ;                                        // the actual function 
      /// flag for nEntries-case 
      bool m_entries ;
      // ======================================================================
    } ;
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
