// ============================================================================
#ifndef LOKI_ALGFUNCTORS_H
#define LOKI_ALGFUNCTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// Gaudi Kernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IAlgorithm.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
// ============================================================================
/** @file
 *
 *  Collection of various functors which test the status of "algorithm".
 *  The idea comes form Gerhard "The Great" Raven
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @see IAlgorithm
 *  @see  Algorithm
 *  @see IAlgManager
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-010-14
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::Algortithms
   *
   *  Collection of various functors which test the status of "algorithm".
   *  The idea comes form Gerhard "The Great" Raven
   *  @see LoKi::Cuts::ALG_PASSED
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-010-14
   */
  namespace Algorithms
  {
    // =========================================================================
    /** @class Passed
     *  simple check of the certain algorithm to pass the filter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Passed : public LoKi::BasicFunctors<void>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Passed ( const std::string& name ) ;
      ~Passed() override;
      /// MANDATORY: clone method ("virtual constructor")
      Passed* clone() const override { return new Passed ( *this ) ; }
      /// MANDATORY: the only one essential method
      result_type operator() () const override ;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    protected:
      // ======================================================================
      /// set the algorithm
      const LoKi::Interface<IAlgorithm>& setAlgorithm
      ( const IAlgorithm* a ) const ;                      // set the algorithm
      /// get the algorithm
      const LoKi::Interface<IAlgorithm>& algorithm () const
      { return m_algorithm ; }                             // get the algorithm
      /// get the algorithm name
      const std::string& algName () const { return m_name ; }
      /// get the algorithm
      const LoKi::Interface<IAlgorithm>&
      getAlgorithm ( const std::string& name ) const ; // get the algorithm
      // ======================================================================
    private:
      // ======================================================================
      /// the algorithm name
      std::string                         m_name      ;
      /// the algorithm itself
      mutable LoKi::Interface<IAlgorithm> m_algorithm ;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Enabled
     *  simple check oif the algorithm is enabled
     *  @see LoKi::Cuts::ALG_ENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Enabled final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Enabled ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Enabled* clone() const { return new Enabled ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Executed
     *  simple check if the algorithm is enabled
     *  @see LoKi::Cuts::ALG_EXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Executed final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Executed ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Executed* clone() const { return new Executed ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Run
     *  A bit complicated action:
     *   - algorithm must be enabled, SUCCESSFULLY executed and passed
     *   - if not executed, execuye it
     *  @see LoKi::Cuts::ALG_RUN
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Run final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Run ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Run* clone() const { return new Run ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AnyPassed
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AnyPassed : public LoKi::BasicFunctors<void>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      AnyPassed ( const std::string& name1 ,
                  const std::string& name2 )
          : AnyPassed( std::vector<std::string>{ name1, name2 } ) {}
      AnyPassed ( const std::string& name1 ,
                  const std::string& name2 ,
                  const std::string& name3 )
          : AnyPassed( std::vector<std::string>{ name1, name2, name3 } ) {}
      AnyPassed ( const std::string& name1 ,
                  const std::string& name2 ,
                  const std::string& name3 ,
                  const std::string& name4 )
          : AnyPassed( std::vector<std::string>{ name1, name2, name3, name4 } ) {}
      AnyPassed ( std::vector<std::string> name ) ;
      AnyPassed ( AnyPassed&& ) = default;
      AnyPassed ( const AnyPassed& ) = default;
      ~AnyPassed() override;
      /// MANDATORY: clone method ("virtual constructor")
      AnyPassed* clone() const override { return new AnyPassed ( *this ) ; }
      /// MANDATORY: the only one essential method
      result_type operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return print ( "ALG_ANYPASSED" , s ) ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// the actual type of vector of algorithms
      bool empty() const { return m_algorithms.empty () ; }
      /// get the algorithm name
      const std::string& algName ( const size_t i ) const { return m_names[i] ; }
      /// get the algorithm
      LoKi::Interface<IAlgorithm>
      getAlgorithm ( const std::string& name ) const ; // get the algorithm
      // ======================================================================
    public:
      // ======================================================================
      virtual std::ostream& print
      ( const std::string&  name ,
        std::ostream&       s    ) const ;
      // ======================================================================
    public:
      // ======================================================================
      typedef std::vector< LoKi::Interface<IAlgorithm> > Algorithms ;
      // ======================================================================
      Algorithms::const_iterator begin () const
      { return m_algorithms.begin () ; }
      Algorithms::const_iterator end   () const
      { return m_algorithms.end   () ; }
      /// get the algorithms
      const Algorithms& algorithms () const { return m_algorithms ; }
      /// get the algorithm name
      const std::vector<std::string>& algNames () const { return m_names ; }
      /// get the algorithms
      void getAlgorithms () const ; // get the algorithms
      // ======================================================================
    private:
      // ======================================================================
      /// the algorithm names
      std::vector<std::string> m_names ;               // the algorithm names
      /// the algorithms themself
      mutable Algorithms m_algorithms ;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AllPassed
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AllPassed final : public AnyPassed
    {
    public:
      // ======================================================================
      // constructor from the algorithm name
      AllPassed ( const std::string& name1 ,
                  const std::string& name2 )
          : AllPassed( std::vector<std::string>{ name1, name2 } ) {}
      AllPassed ( const std::string& name1 ,
                  const std::string& name2 ,
                  const std::string& name3 )
          : AllPassed( std::vector<std::string>{ name1, name2, name3 } ) {}
      AllPassed ( const std::string& name1 ,
                  const std::string& name2 ,
                  const std::string& name3 ,
                  const std::string& name4 )
          : AllPassed( std::vector<std::string>{ name1, name2, name3, name4 } ) {}
      AllPassed ( std::vector<std::string> names )
          : AuxFunBase{ std::tie( names ) }
          , AnyPassed( std::move(names) ) { }
      /// MANDATORY: clone method ("virtual constructor")
      AllPassed* clone() const override { return new AllPassed ( *this ) ; }
      /// MANDATORY: the only one essential method
      result_type operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return print ( "ALG_ANYPASSED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AnyEnabled
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AnyEnabled final : public AnyPassed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      AnyEnabled ( const std::string& name1 ,
                   const std::string& name2 )
          : AnyEnabled( std::vector<std::string>{ name1, name2 } ) {}
      AnyEnabled ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 )
          : AnyEnabled( std::vector<std::string>{ name1, name2, name3 } ) {}
      AnyEnabled ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 ,
                   const std::string& name4 )
          : AnyEnabled( std::vector<std::string>{ name1, name2, name3, name4 } ) {}
      AnyEnabled ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , AnyPassed( std::move(names) ) { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      AnyEnabled* clone() const override { return new AnyEnabled ( *this ) ; }
      /// MANDATORY: the only one essential method
      result_type operator() () const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return print ( "ALG_ANYENABLED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AllEnabled
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AllEnabled final : public AnyPassed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      AllEnabled ( const std::string& name1 ,
                   const std::string& name2 )
          : AllEnabled( std::vector<std::string>{ name1, name2 } ) {}
      AllEnabled ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 )
          : AllEnabled( std::vector<std::string>{ name1, name2, name3 } ) {}
      AllEnabled ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 ,
                   const std::string& name4 )
          : AllEnabled( std::vector<std::string>{ name1, name2, name3, name4 } ) {}
      AllEnabled ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , AnyPassed( std::move(names) ) { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      virtual  AllEnabled* clone() const { return new AllEnabled ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_ALLENABLED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AnyExecuted
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AnyExecuted : public AnyPassed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      AnyExecuted ( const std::string& name1 ,
                    const std::string& name2 )
          : AnyExecuted( std::vector<std::string>{ name1, name2 } ) {}
      AnyExecuted ( const std::string& name1 ,
                    const std::string& name2 ,
                    const std::string& name3 )
          : AnyExecuted( std::vector<std::string>{ name1, name2, name3 } ) {}
      AnyExecuted ( const std::string& name1 ,
                    const std::string& name2 ,
                    const std::string& name3 ,
                    const std::string& name4 )
          : AnyExecuted( std::vector<std::string>{ name1, name2, name3, name4 } ) {}
      AnyExecuted ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , AnyPassed( std::move(names) ) { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      virtual  AnyExecuted* clone() const { return new AnyExecuted ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_ANYEXECUTED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class AllExecuted
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API AllExecuted : public AnyPassed
    {
    public:
      // ======================================================================
      /// forward to base class constructor
      AllExecuted ( std::string name1 ,
                    std::string name2 )
          : AllExecuted( std::vector<std::string>{ std::move(name1), std::move(name2) } ) {}
      AllExecuted ( std::string name1 ,
                    std::string name2 ,
                    std::string name3 )
          : AllExecuted( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) {}
      AllExecuted ( std::string name1 ,
                    std::string name2 ,
                    std::string name3 ,
                    std::string name4 )
          : AllExecuted( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) {}
      AllExecuted ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , AnyPassed( std::move(names) ) { }
      /// MANDATORY: clone method ("virtual constructor")
      AllExecuted* clone() const override { return new AllExecuted ( *this ) ; }
      /// MANDATORY: the only one essential method
      result_type operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return print ( "ALG_ALLEXECUTED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class RunAll
     *  @see LoKi::Cuts::ALG_RUNALL
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API RunAll : public AllExecuted
    {
    public:
      // ======================================================================
      RunAll ( std::string name1 ,
               std::string name2 )
          : RunAll( std::vector<std::string>{ std::move(name1), std::move(name2) } ) {}
      RunAll ( std::string name1 ,
               std::string name2 ,
               std::string name3 )
          : RunAll( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) {}
      RunAll ( std::string name1 ,
               std::string name2 ,
               std::string name3 ,
               std::string name4 )
          : RunAll( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) {}
      RunAll ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , AllExecuted( std::move(names) ) { }
      /// MANDATORY: clone method ("virtual constructor")
      virtual  RunAll* clone() const { return new RunAll ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_RUNALL" , s ) ; }
      // ======================================================================
    } ;
    // =========================================================================
    /** @class NumPassed
     *  Simple functor to count number of passed algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    class GAUDI_API NumPassed : public LoKi::BasicFunctors<void>::Function
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      NumPassed ( std::string name1 ,
                  std::string name2 )
        : NumPassed( std::vector<std::string>{ std::move(name1), std::move(name2) } ) {}
      /// constructor from the algorithm name
      NumPassed ( std::string name1 ,
                  std::string name2 ,
                  std::string name3 )
        : NumPassed( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) {}
      /// constructor from the algorithm name
      NumPassed ( std::string name1 ,
                  std::string name2 ,
                  std::string name3 ,
                  std::string name4 )
        : NumPassed( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) {}
      /// constructor from the algorithm names
      NumPassed ( std::vector<std::string> names )
        : AuxFunBase{ std::tie(names) }
        , m_fun{ std::move(names) } { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      virtual  NumPassed* clone() const { return new NumPassed ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_NUMPASSED" , s ) ; }
      // ======================================================================
    public:
      // ======================================================================
      virtual std::ostream& print
      ( const std::string& name ,
        std::ostream& s ) const { return m_fun.print ( name , s ) ; }
      // ======================================================================
    public:
      // ======================================================================
      AnyPassed::Algorithms::const_iterator begin () const
      { return m_fun.begin () ; }
      AnyPassed::Algorithms::const_iterator end   () const
      { return m_fun.end   () ; }
      /// get the algorithms
      const AnyPassed::Algorithms& algorithms () const
      { return m_fun.algorithms() ; }
      /// get the algorithm name
      const std::vector<std::string>& algNames () const
      { return m_fun.algNames() ; }
      /// get the algorithms
      void getAlgorithms () const { m_fun.getAlgorithms () ; }
      // =======================================================================
    private:
      // =======================================================================
      /// the actual functor
      LoKi::Algorithms::AnyPassed m_fun ;                  // the actual functor
      // =======================================================================
    } ;
    // =========================================================================
    /** @class NumEnabled
     *  Simple functor to count number of enabled algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    class GAUDI_API NumEnabled  : public NumPassed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      NumEnabled ( std::string name1 ,
                   std::string name2 )
          : NumEnabled( std::vector<std::string>{ std::move(name1), std::move(name2) } ) { }
      /// constructor from the algorithm name
      NumEnabled ( std::string name1 ,
                   std::string name2 ,
                   std::string name3 )
          : NumEnabled( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) { }
      /// constructor from the algorithm name
      NumEnabled ( std::string name1 ,
                   std::string name2 ,
                   std::string name3 ,
                   std::string name4 )
          : NumEnabled( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) { }
      /// constructor from the algorithm name
      NumEnabled ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , NumPassed( std::move(names) ) { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      virtual  NumEnabled* clone() const { return new NumEnabled ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_NUMENABLED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @class NumExecuted
     *  Simple functor to count number of executed algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    class GAUDI_API NumExecuted  : public NumPassed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      NumExecuted ( std::string name1 ,
                    std::string name2 )
          : NumExecuted( std::vector<std::string>{ std::move(name1), std::move(name2) } ) { }
      /// constructor from the algorithm name
      NumExecuted ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 )
          : NumExecuted( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) { }
      /// constructor from the algorithm name
      NumExecuted ( const std::string& name1 ,
                   const std::string& name2 ,
                   const std::string& name3 ,
                   const std::string& name4 )
          : NumExecuted( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) { }
      /// constructor from the algorithm name
      NumExecuted ( std::vector<std::string> names )
          : AuxFunBase{ std::tie(names) }
          , NumPassed( std::move(names) ) { }
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      virtual  NumExecuted* clone() const { return new NumExecuted ( *this ) ; }
      /// MANDATORY: the only one essential method
      virtual result_type operator() () const ;
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream ( std::ostream& s ) const
      { return print ( "ALG_NUMEXECUTED" , s ) ; }
      // ======================================================================
    } ;
    // ========================================================================
  } //                                        end of namespace LoKi::Algorithms
  // ==========================================================================
} //                                                     end of namaespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_ALGFUNCTORS_H
// ============================================================================
