// $Id: Holder.h 133878 2012-01-18 19:33:38Z ibelyaev $
// ============================================================================
#ifndef LOKI_HOLDER_H 
#define LOKI_HOLDER_H 1
// ============================================================================
// Include files
// ============================================================================
#include "LoKi/Reference.h"
#include "LoKi/Field.h"
// ============================================================================
/** @file LoKi/Holder.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *   By usage of this code one clearly states the disagreement 
 *    with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 * 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
namespace LoKi 
{
  // ==========================================================================
  /** @class Holder Holder.h LoKi/Holder.h
   *  Helper class to treat 2-argument functions 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-06-08
   */
  template <class TYPE1,class TYPE2>
  struct Holder 
    : public std::pair <typename LoKi::Field<const TYPE1>::Type,
                        typename LoKi::Field<const TYPE2>::Type>
  {
  public:
    typedef typename  LoKi::Field<const TYPE1>::Type         First  ;
    typedef typename  LoKi::Field<const TYPE2>::Type         Second ;
    typedef std::pair<First,Second>                          Pair   ;
  public:
    /// constructor from data fields (including the default) 
    Holder ( const TYPE1& f , const TYPE2& s  ) 
      : Pair ( First ( f ) , Second ( s ) ) {}
    /// constructor from the base 
    Holder ( const Pair & base   ) : Pair ( base ) {}
    /// default constructor 
    Holder ( )                     : Pair ( )      {}
    /// copy constuctor 
    Holder ( const Holder& copy  ) : Pair ( copy ) {}
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
#endif // LOKI_HOLDER_H
// ============================================================================
