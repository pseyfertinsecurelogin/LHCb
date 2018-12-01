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
#ifndef LOKI_DUMPER_H
#define LOKI_DUMPER_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
#include "LoKi/BasicFunctors.h"
#include "LoKi/Primitives.h"
#include "LoKi/Dump.h"
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
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-28
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Functors
  {
    // ========================================================================
    /** @class Dump1_
     *  simple class to simplify the monitoring of functors
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2012-01-28
     */
    template <class TYPE1,class TYPE2>
    class Dump1_ : public LoKi::Functor<TYPE1,TYPE2>
    {
    private:
      // ======================================================================
      typedef LoKi::Functor<TYPE1,TYPE2>                               Fun_ ;
      typedef typename Fun_::argument                              argument ;
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from the dumper
       *  @param fun   the functor
       *  @param dump  the dumper
       *  @param stream the stream
       */
      Dump1_ ( LoKi::Assignable_t<Fun_>          fun                ,
               const LoKi::Dump&                 dump               ,
               const bool                        right  = true      ,
               std::ostream&                     stream = std::cout )
        : LoKi::AuxFunBase ( std::tie ( fun , dump , right ) )
        , m_fun    ( std::move(fun) )
        , m_right  ( right  )
        , m_stream ( stream )
        , m_dump   ( dump   )
      {}
      /** constructor
       *  @param fun   the functor
       *  @param open  the opening
       *  @param clone the closing
       *  @param stream the stream
       */
      Dump1_ ( LoKi::Assignable_t<Fun_>          fun                ,
               const std::string&                open   = ""        ,
               const std::string&                close  = "\n"      ,
               const bool                        right  = true      ,
               std::ostream&                     stream = std::cout )
      : Dump1_( std::move(fun), { open, close }, right, stream )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      Dump1_* clone() const override { return new Dump1_ ( *this ) ; }
      /// MANDATORY: the only one essential method
      TYPE2 operator() ( argument a ) const override;
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    private :
      // ======================================================================
      /// the functor
      LoKi::Assignable_t<Fun_>               m_fun    ;
      /// right-action ?
      bool                                   m_right  ; // right action?
      /// the stream
      std::ostream&                          m_stream ; // the stream
      /// the dumper
      LoKi::Dump                             m_dump   ; // the dumper
      // ======================================================================
    } ;
    // ========================================================================
    template <class TYPE1,class TYPE2>
    TYPE2
    LoKi::Functors::Dump1_<TYPE1,TYPE2>::operator()
      ( typename LoKi::Functor<TYPE1,TYPE2>::argument a ) const
    {
      if ( this->m_right )
      {
        this -> m_stream << this -> m_dump.open  () ;
        Gaudi::Utils::toStream ( a , this->m_stream ) ;
        this -> m_stream << this -> m_dump.close () ;
        //
        return this->m_fun.fun ( a ) ;
      }
      //
      auto res = this->m_fun.fun ( a ) ;
      //
      this -> m_stream << this -> m_dump.open  () ;
      Gaudi::Utils::toStream ( a , this->m_stream ) ;
      this -> m_stream << this -> m_dump.close () ;
      //
      return res ;
    }
    // ========================================================================
    template <class TYPE1,class TYPE2>
    std::ostream&
    LoKi::Functors::Dump1_<TYPE1,TYPE2>::fillStream ( std::ostream& s ) const
    {
      s << " dump1 ( " << this->m_fun ;
      if ( !this -> m_right ) { s << " ,False " ; }
      return s << " )";
    }
    // ========================================================================
    template <>
    bool Dump1_<double,bool>::operator() ( double a ) const ;
    // ========================================================================
    template <>
    double Dump1_<double,double>::operator() ( double a ) const ;
    // ========================================================================
  } //                                          end of namesapce LoKi::Functors
  // ==========================================================================
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Dump1_<TYPE1,TYPE2>
  dump1 ( F&&                               fun                  ,
          const bool                        right = true         ,
          const LoKi::Dump&                 dump  = LoKi::Dump() )
  { return { std::forward<F>(fun) , dump , right } ; }
  // ==========================================================================
  template <class TYPE2>
  const LoKi::Functor<void,TYPE2>&
  dump1 ( const LoKi::Functor<void,TYPE2>&    fun      ,
          const bool                       /* right */ ,
          const LoKi::Dump&                /* dump  */ ) { return fun ; }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_DUMPER_H
// ============================================================================
