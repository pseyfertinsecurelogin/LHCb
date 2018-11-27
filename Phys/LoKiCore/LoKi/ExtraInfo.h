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
#ifndef LOKI_EXTRAINFO_H
#define LOKI_EXTRAINFO_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <limits>
#include <climits>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Info.h"
// ============================================================================
/** @file LoKi/ExtraInfo.h
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
 */
namespace LoKi
{
  // ==========================================================================
  namespace ExtraInfo
  {
    // ========================================================================
    /** @var defaultValue
     *  the default value for missing keys
     *  @author Vanya BELAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-14
     */
    const double defaultValue = -1*std::numeric_limits<double>::max() ;
    // ========================================================================
    /** @class GetInfo
     *  simple function which return the value of "extra info",
     *  for classes equipped with "extraInfo" methods, e.g.
     *  LHCb::Particle, LCHb::Vertex, LHCb::Track
     *  @author Vanya BELYAEV ibelyaev@physics.sye.edu
     *  @date 2007-06-14
     */
    template <class TYPE, class TYPE2=double>
    class GetInfo : public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the index and the default value
      GetInfo ( const int index , T2 value = defaultValue )
        : LoKi::AuxFunBase ( std::tie ( index , value ) )
        , m_index   ( index )
        , m_default ( value )
      { }
      /// MANDATORY: clone method ("virtual constructor")
      GetInfo* clone() const override { return new GetInfo(*this); }
      /// MANDATORY: the only one essential method
      TYPE2 operator()
        ( typename LoKi::Functor<TYPE,TYPE2>::argument a ) const override
      { return LoKi::ExtraInfo::info ( a , m_index , m_default ) ; }
      /** OPTIONAL: the nice printout
       *  @attention: it is worth to redefine it for each type
       */
      std::ostream& fillStream ( std::ostream& s ) const override
      { return s << this->objType() << "(" << m_index << "," << m_default << ")" ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get the index
      int     index  () const { return m_index   ; }
      double  value  () const { return m_default ; }
      // ======================================================================
    private:
      // ======================================================================
      /// index to be searched
      int    m_index   ;                                // index to be searched
      /// default value for non-existing index
      TYPE2  m_default ;                // default value for non-existing index
      // ======================================================================
    };
       // ========================================================================
    /** @class CheckInfo
     *  simple predicate to check the existence of "key" in the "extraInfo"
     *  for classes equipped with "extraInfo" methods
     *  @author Vanya BELYAEV ibelyaev@physics.sye.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    class CheckInfo : public LoKi::Functor<TYPE,bool>
    {
    public:
      // ======================================================================
      /// constructor fomr the index and default value
      explicit CheckInfo ( const int    index )
        : LoKi::AuxFunBase ( std::tie ( index ) )
        , m_index   ( index )
      { }
      /// MANDATORY: clone method ("virtual constructor")
      CheckInfo* clone() const override { return new CheckInfo (*this); }
      /// MANDATORY: the only one essential method
      bool operator()
        ( typename LoKi::Functor<TYPE,bool>::argument a )  const override
      { return LoKi::ExtraInfo::hasInfo ( a , m_index ) ; }
      /** OPTIONAL: the nice printout
       *  @attention: it is worth to redefine for each type
       */
      std::ostream& fillStream ( std::ostream& s ) const override
      { return s <<  this->objType() << "(" << m_index << ")" ; }
      // ======================================================================
      /// no assigenement
      CheckInfo& operator= ( CheckInfo& ) = delete;          // no assigenement
      // ======================================================================
    public:
      // ======================================================================
      /// get the index
      int index() const { return m_index ; }
      // ======================================================================
    private:
      // ======================================================================
      /// index to be searched
      int    m_index   ;                               // index to be searched
      // ======================================================================
    };
    // ========================================================================
    /** @class GetSmartInfo
     *
     *  Simple class which checks the "extraInfo" field of the
     *  argument
     *  for the given key, and return the corresponding value.
     *  For the missing keys the infomation is evaluated using the
     *  supplied function and (optionally) inserted into "extraInfo"
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-14
     */
    template <class TYPE, class TYPE2=double>
    class GetSmartInfo : public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      /// contructor from ID, function and the flag
      GetSmartInfo ( const int                            index          ,
                     LoKi::FunctorFromFunctor<TYPE,TYPE2> fun            ,
                     const bool                           update = false )
        : LoKi::AuxFunBase ( std::tie ( index , fun , update ) )
        , m_fun    ( std::move(fun) )
        , m_index  ( index  )
        , m_update ( update )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      GetSmartInfo* clone() const override { return new GetSmartInfo (*this); }
      /// MANDATORY: the only one essential method
      TYPE2 operator()
      ( typename LoKi::Functor<TYPE,TYPE2>::argument a ) const override
      {
        // check extra info:
        if ( LoKi::ExtraInfo::hasInfo ( a , m_index ) )
        { return LoKi::ExtraInfo::info ( a , m_index , defaultValue  ) ; }   // RETURN
        // evaluate the function
        const double result = m_fun.fun ( a ) ;
        // update info (if needed)
        if ( m_update ) { LoKi::ExtraInfo::addInfo( a , m_index , result ) ; }
        //
        return result ;                                             // RETURN
      }
      /** OPTIONAL: the nice printout
       *  @attention: it is worth to redefine for each type
       */
      std::ostream& fillStream ( std::ostream& s ) const override
      {
        s << "info(" << m_index << "," << m_fun << "," ;
        return s <<  ( m_update ? "True" : "False"  ) << ")" ;
      }
      /** unique function ID (hash); see LoKi::genericID
       *  @attention the key is used and funtion-ID
       *  @see LoKi::AuxFunBase
       *  @return the function-ID
       */
      std::size_t         id     () const override { return m_index ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get the index
      int                         index  () const { return m_index  ; }
      /// get the function
      const auto& func () const { return m_fun.func () ; }
      /// get the flag
      bool                        update () const { return m_update ; }
      // ======================================================================
      /// no assignement
      GetSmartInfo& operator=( const GetSmartInfo& ) = delete; // no assignment
      // ======================================================================
    private:
      // ======================================================================
      /// the function to be evaluated (if needed)
      LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ; // the function to be evaluated (if needed)
      /// the index ("ID") in "extraInfo" table
      int   m_index; // the index in "extraInfo" table
      /// "update"-flag (indicate the insertion of "extraInfo" for missing data
      bool  m_update ; // "update flag"
      // ======================================================================
    } ;
    // ========================================================================
  } // end of namespace LoKi::ExtraInfo
  // ==========================================================================
  namespace ExtraInfo2
  {
    // ========================================================================
    /** @class LogInfo
     *  Simple function to "log"-info
     *  for classes equipped with "extraInfo" methods
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-12-09
     */
    template <class TYPE,class TYPE2=double>
    class LogInfo final : public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      /// constructor from the index and default value
      LogInfo ( const LoKi::FunctorFromFunctor<TYPE,TYPE2> fun   ,
                const int                                  index )
        : LoKi::AuxFunBase ( std::tie ( fun , index ) )
        , m_fun     ( std::move(fun) )
        , m_index   ( index )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      LogInfo* clone() const override { return new LogInfo (*this); }
      /// MANDATORY: the only one essential method
      TYPE2 operator()
        ( typename LoKi::Functor<TYPE,TYPE2>::argument a )  const override
      {
        TYPE2 result = m_fun.fun ( a ) ;
        //
        LoKi::ExtraInfo::addInfo ( a , m_index , result ) ;
        //
        return result ;
        //
      }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return s << "logging (" << m_fun << ","  << m_index << ")" ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual functor
      const LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ; // the functor
      /// index to be searched
      int    m_index   ;                                 // the index
      // ======================================================================
    };
    // ========================================================================
  } //                                        end of namespace LoKi::ExtraInfo2
  // ==========================================================================
  /** simple function for "on-flight" construction of the "smart-extra-info"
   *  function
   *
   *  The concept is inspired by Jose Angel Hernando Morata and Hugo Ruiz Peres
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-12-09
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::ExtraInfo::GetSmartInfo<TYPE,TYPE2> info( int    index          ,
                                                  F&&    functor        ,
                                                  bool   update = false )
  { return { index , std::forward<F>(functor) , update } ; }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_EXTRAINFO_H
// ============================================================================
