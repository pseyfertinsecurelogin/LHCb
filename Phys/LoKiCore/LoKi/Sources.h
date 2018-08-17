// ============================================================================
#ifndef LOKI_SOURCES_H
#define LOKI_SOURCES_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/DataObject.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GetData.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Assert.h"
#include "LoKi/Services.h"
#include "LoKi/Functor.h"
#include "LoKi/TES.h"
// ============================================================================
/** @file LoKi/Sources.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-17
 */
namespace LoKi
{
  // ==========================================================================
  namespace Functors
  {
    // ========================================================================
    /** @class Sources LoKi/Sources.h
     *
     *  The concept belongs to the Gerhard "The Great" Raven.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-11-28
     */
    template <class TYPE                           ,
              class TYPE2=std::vector<TYPE*>       ,
              class TYPE3=typename TYPE::Container >
    class Source
      : public LoKi::Functor<void,TYPE2>
      , public LoKi::TES::Get        
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the context algorithm, path in TES and root-in-tes  flag
      Source
      ( const GaudiAlgorithm* algorithm           ,
        const std::string&    path                , 
        const bool            useRootInTES = true ) 
        : LoKi::AuxFunBase ( std::tie ( algorithm , path , useRootInTES ) )
        , LoKi::Functor<void,TYPE2> () 
        , LoKi::TES::Get   ( algorithm , path , useRootInTES ) 
      {}
      /// constructor from the service and path in TES
      Source
      ( const IDataProviderSvc*  svc   ,
        const std::string&       path  )
        : LoKi::AuxFunBase ( std::tie ( svc , path ) )
        , LoKi::Functor<void,TYPE2> () 
        , LoKi::TES::Get   ( svc , path ) 
      {}      
      /// copy constructor
      Source ( const Source& right )  = default;
      /// MANDATORY: clone method ("virtual constructor")
      Source* clone() const override { return new Source ( *this ) ; }
      /// MANATORY: the only one essenial method
      TYPE2 operator() ( ) const override
      {
        /// get the data from TES:
        const TYPE3* data = LoKi::TES::get_<TYPE3>( *this ) ;
        /// check the validity:
        Assert ( data , "No valid data is found at '" + this->path() + "'" ) ;
        /// return the valid data
        return { data->begin() , data->end() } ;
      }
      // ======================================================================
    } ;
    // ========================================================================
  } //                                          end of namespace LoKi::Functors
  // ==========================================================================
  /** simple "source" function
   *  @code
   *  std::vector<TYPE*> out = source<TYPE>
   *
   *   // some 'tee'-action:
   *   const LoKi::Functor<TYPE,TYPE2>& fun = ...;
   *
   *   std::vector<TYPE> out = in >> tee ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to the Gerhard "The Great" Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <class TYPE>
  inline
  LoKi::Functors::Source<TYPE>
  source ( const std::string&      path ,
           const IDataProviderSvc* svc  )
  { return LoKi::Functors::Source<TYPE>( svc , path ) ; }
  // ==========================================================================
  template <class TYPE>
  inline
  LoKi::Functors::Source<TYPE>
  source ( const std::string&    path                ,
           const GaudiAlgorithm* algorithm           ,
           const bool            useRootInTES = true )
  { return LoKi::Functors::Source<TYPE>( algorithm , path , useRootInTES ) ; }
  // ==========================================================================
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_SOURCES_H
// ============================================================================
