// ============================================================================
#ifndef RELATIONS_GET_H
#define RELATIONS_GET_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <set>
#include <algorithm>
#include <numeric>
// ============================================================================
// Relations
// ============================================================================
#include "Relations/RelationTypeTraits.h"
#include "Relations/RelationWeightedTypeTraits.h"
// ============================================================================
/** @file
 *
 *  The collection of trivial utilities to manipulate with
 *  relation tabels/ranges/iterators
 *
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-21
 */
// ============================================================================
namespace Relations
{
  // ==========================================================================
  /** simple function to extract all values of "TO"
   *  from relation Ranges into the plain container
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloDigit,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  const LCHb::CaloDigit* digit = ... ;
   *  // get all links from the given LHCb::CaloDigit obejct:
   *  Table::Range links = table->relations( digit ) ;
   *
   *  // get all MC-particles into one plain container
   *  typedef std:vector<const LHCb::MCParticle*> MCPARTICLES ;
   *  MCPARTICLES mcparticles ;
   *  Relations::getTo ( links.begin () ,
   *                     links.end   () ,
   *                     std::back_inserter( mcparticles ) ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *
   *  For extraction of the list of "unique" objects see getUniqueTo
   *  @see Relations::getUniqueTo
   *
   *  @param first 'begin'-iterator for the sequence of input links
   *  @param last  'end'-iterator for the sequence of input links
   *  @param output the posision of the output iterator
   *  @return the updated position fo output iterator
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINK,class OUTPUT>
  inline OUTPUT getTo
  ( LINK   first  ,
    LINK   last   ,
    OUTPUT output )
  {
    using arg_t = decltype(*first);
    return std::transform( first, last, output, [](arg_t arg) { return arg.to(); } );
  }
  // ==========================================================================
  /** simple function to extract all values of "TO"
   *  from relation Ranges into the plain container
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloDigit,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  const LCHb::CaloDigit* digit = ... ;
   *  // get all links from the given LHCb::CaloDigit obejct:
   *  Table::Range links = table->relations( digit ) ;
   *
   *  // get all MC-particles into one plain container
   *  typedef std:vector<const LHCb::MCParticle*> MCPARTICLES ;
   *  MCPARTICLES mcparticles ;
   *  Relations::getTo ( links , std::back_inserter( mcparticles ) ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *
   *  @param links  the sequence of input links
   *  @param output the posision of the output iterator
   *  @return the updated position fo output iterator
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINKS,class OUTPUT>
  inline OUTPUT getTo
  ( LINKS  links  ,
    OUTPUT output )
  { return getTo ( links.begin() , links.end() , output ) ; }
  // ==========================================================================
  /** simple function to extract all values of "WEIGHT"
   *  from relation Ranges into the plain container
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloDigit,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  const LCHb::CaloDigit* digit = ... ;
   *  // get all links from the given LHCb::CaloDigit obejct:
   *  Table::Range links = table->relations( digit ) ;
   *
   *  // get all weight into one plain container
   *  typedef std:vector<double> WEIGHTS ;
   *  WEIGHTS weight ;
   *  Relations::getWeight ( links.begin () ,
   *                         links.end   () ,
   *                         std::back_inserter( weights ) ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *
   *  @param first 'begin'-iterator for the sequence of input links
   *  @param last  'end'-iterator for the sequence of input links
   *  @param output the posision of the output iterator
   *  @return the updated position fo output iterator
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINK,class OUTPUT>
  inline OUTPUT getWeight
  ( LINK   first  ,
    LINK   last   ,
    OUTPUT output )
  {
    using arg_t = decltype(*first);
    return std::transform(first,last,output,[](arg_t arg) { return arg.weight(); } );
  }
  // ==========================================================================
  /** Simple function to extract all values of "WEIGHT"
   *  from relation Ranges into the plain container
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloDigit,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  const LCHb::CaloDigit* digit = ... ;
   *  // get all links from the given LHCb::CaloDigit obejct:
   *  Table::Range links = table->relations( digit ) ;
   *
   *  // get all weight into one plain container
   *  typedef std:vector<double> WEIGHTS ;
   *  WEIGHTS weight ;
   *  Relations::getWeight ( links , std::back_inserter( weights ) ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *
   *  @param links the sequence of input links
   *  @param output the posision of the output iterator
   *  @return the updated position fo output iterator
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINKS,class OUTPUT>
  inline OUTPUT getWeight
  ( LINKS  links  ,
    OUTPUT output )
  { return getWeight ( links.begin() , links.end() , output ) ; }
  // ==========================================================================
  /** Trivial algorithm whcih allow to accumulate the
   *  "WEIGHT" from the relations ranges
   *
   *  @code
   *
   *  Range links = .... ;
   *
   *  // find a sum of all weights
   *  const double weight =
   *    Relations::sumWeight( links.begin() , links.end() , 0.0 )
   *
   *  @endcode
   *
   *  @param first 'begin'-iterator for the sequence of input links
   *  @param last  'end'-iterator for the sequence of input links
   *  @param weight the initial value for the accumulated weight
   *  @return the updated value for the accumulated weight
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINK,class WEIGHT>
  inline WEIGHT sumWeight
  ( LINK   first  ,
    LINK   last   ,
    WEIGHT weight )
  {
    static const std::plus<WEIGHT> _plus ;
    using arg_t = decltype(*first);
    return std::accumulate(first,last, weight,
                           [&](WEIGHT w, arg_t arg)
                           { return _plus(w, arg.weight); });
  }
  // ==========================================================================
  /** Simple function to extract all *UNIQUE* values of "TO"
   *  from relation Ranges into the plain container
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloCluster,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  // get ALL links
   *  Table::Range links = table->relations() ;
   *
   *  // get all *UNIQUE* MC-particles into one plain container
   *  /// not very efficient:
   *  // typedef std:vector<const LHCb::MCParticle*> MCPARTICLES ;
   *  /// efficient:
   *  typedef std:vector<const LHCb::MCParticle*> MCPARTICLES ;
   *
   *  MCPARTICLES mcparticles ;
   *  Relations::getUniqueTo ( links.begin () ,
   *                           links.end   () ,
   *                           mcparticles    ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *
   * @attention IMPORTANT
   * // ///////////////////////////////////////////////////////////////////////
   * // IMPORTANT!!!
   * // This utility/function/algorihtm is not very efficient for
   * // "std::vector" containers. but it is efficient for std::set
   * // containers:
   * // //////////////////////////////////////////////////////////////////////
   *
   *  @param first 'begin'-iterator for the sequence of input links
   *  @param last  'end'-iterator for the sequence of input links
   *  @param cont  the contained to be updated
   *  @return the number of added unique elements
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINK,class CONTAINER>
  inline size_t getUniqueTo
  ( LINK       first  ,
    LINK       last   ,
    CONTAINER& cont   )
  {
    // store the current size
    const size_t size = cont.size() ;
    getTo( first, last, std::back_inserter(cont) );
    const auto begin = cont.begin () ;
    const auto end   = cont.end   () ;
    // sort the subsequence
    std::sort( begin , end ) ;
    // remove duplicates
    cont.erase( std::unique ( begin , end ), end );
    // return the new container size
    return cont.size() - size ;
  }
  // ==========================================================================
  /** Simple function to extract all *UNIQUE* values of "TO"
   *  from relation Ranges into the "std::set" container
   *  (It is a partial specialization of gfeneric getUniqueTo function)
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloCluster,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  // get ALL links
   *  Table::Range links = table->relations( cluster ) ;
   *
   *  // get all *UNIQUE* MC-particles into one plain container
   *  typedef std::set<const LHCb::MCParticle*> MCPARTICLES ;
   *  MCPARTICLES mcparticles ;
   *
   *  Relations::getUniqueTo ( links.begin () ,
   *                           links.end   () ,
   *                           mcparticles    ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *  @see Relations::getUniqueTo
   *  @see Relations::getTo
   *  @see std:set
   *
   *  @param  first 'begin'-iterator for the sequence of input links
   *  @param  last  'end'-iterator for the sequence of input links
   *  @param  cont  the set to be updated
   *  @return the number of added unique elements
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINK,class OBJECT>
  inline size_t getUniqueTo
  ( LINK              first  ,
    LINK              last   ,
    std::set<OBJECT>& cont   )
  {
    // store the current size
    const size_t size = cont.size() ;
    getTo(first,last,std::inserter(cont, cont.end()));
    // return number of newly added elements
    return cont.size() - size ;
  }
  // ==========================================================================
  /** Simple function to extract all *UNIQUE* values of "TO"
   *  from relation Ranges into the "std::set" container
   *  (It is a partial specialization of gfeneric getUniqueTo function)
   *
   *  @code
   *
   *  typedef IRelationWeighted<LHCb::CaloCluster,LHCb::MCParticle,float> Table ;
   *  // get the relation table:
   *  const ITable* table = ...  ;
   *
   *  // get ALL links
   *  Table::Range links = table->relations( cluster ) ;
   *
   *  // get all *UNIQUE* MC-particles into one plain container
   *  // e.g.: typedef std::set<const LHCb::MCParticle*> MCPARTICLES ;
   *  typedef std::vector<const LHCb::MCParticle*> MCPARTICLES ;
   *  MCPARTICLES mcparticles ;
   *
   *  Relations::getUniqueTo ( links , mcparticles    ) ;
   *
   *  @endcode
   *
   *  @see IRelationWeighted
   *  @see Relations::getUniqueTo
   *  @see Relations::getTo
   *  @see std:set
   *
   *  @param  link the sequence of input links
   *  @param  cont  the set to be updated
   *  @return the number of added unique elements
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINKS,class CONTAINER>
  inline size_t getUniqueTo
  ( LINKS      links  ,
    CONTAINER& cont   )
  { return getUniqueTo ( links.begin() , links.end() , cont ) ; }
  // ==========================================================================
  /** Trivial algorithm whcih allow to accumulate the
   *  "WEIGHT" form relations ranges
   *
   *  @code
   *
   *  Range links = .... ;
   *
   *  // find a sum of all weights
   *  const double weight =
   *    Relations::sumWeight( links.begin() , links.end() , 0.0 )
   *
   *  @endcode
   *
   *  @param links the sequence of input links
   *  @param weight the initial value for the accumulated weight
   *  @return the updated value for the accumulated weight
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-21
   */
  template <class LINKS,class WEIGHT>
  inline WEIGHT sumWeight
  ( LINKS  links  ,
    WEIGHT weight )
  { return sumWeight( links.begin() , links.end() , weight ) ; }
  // ==========================================================================
  template <class ITERATOR, class COMPARE, class OUTPUT>
  inline OUTPUT  _getUniqueFrom
  ( ITERATOR first  ,
    ITERATOR last   ,
    COMPARE  cmp    ,
    OUTPUT   output )
  {
    if (first!=last) {
      *output++ = first->from();
      using arg_t = decltype(*first);
      std::mismatch( std::next(first), last, first,
                     [&](arg_t curr, arg_t prev) {
                       if (cmp(prev,curr)) *output++ = curr.from();
                       return true;  // make sure there is no mismatch ;-)
                     } );
    }
    return output;
  }
  // ==========================================================================
  /** Trivial function to get all "FROM" elements form the sequnce
   *  @attention the sequence MUST be properly sorted
   *
   *  @code
   *
   *   Range links = ... ;
   *
   *   std::vector<FROM> res = ... ;
   *
   *   getUniqueFrom ( links.begin () ,
   *                   links.end   () ,
   *                   std::back_inserter ( res ) ) ;
   *
   *  @endcode
   *
   *  @param first  (INPUT)  begin of the sequnce
   *  @param last   (INPUT)  the end  of the sequnce
   *  @param output (OUTPUT) the outptu iterator
   *  @return advanced output iterator
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2009-05-24
   */
  template <class FROM, class TO, class OUTPUT>
  inline OUTPUT  getUniqueFrom
  ( typename Relations::RelationTypeTraits<FROM,TO>::iterator first ,
    typename Relations::RelationTypeTraits<FROM,TO>::iterator last  ,
    OUTPUT  output )
  {
    return _getUniqueFrom
      ( first ,
        last  ,
        typename Relations::RelationTypeTraits<FROM,TO>::LessByFrom()  ,
        output ) ;
  }
  // ==========================================================================
  /** Trivial function to get all "FROM" elements form the sequnce
   *  @attention the sequence MUST be properly sorted
   *
   *  @code
   *
   *   Range links = ... ;
   *
   *   std::vector<FROM> res = ... ;
   *
   *   getUniqueFrom ( links.begin () ,
   *                   links.end   () ,
   *                   std::back_inserter ( res ) ) ;
   *
   *  @endcode
   *
   *  @param first  (INPUT)  begin of the sequnce
   *  @param last   (INPUT)  the end  of the sequnce
   *  @param output (OUTPUT) the outptu iterator
   *  @return advanced output iterator
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2009-05-24
   */
  template <class FROM, class TO, class WEIGHT,  class OUTPUT>
  inline OUTPUT  getUniqueFrom
  ( typename Relations::RelationWeightedTypeTraits<FROM,TO,WEIGHT>::iterator first ,
    typename Relations::RelationWeightedTypeTraits<FROM,TO,WEIGHT>::iterator last  ,
    OUTPUT   output )
  {
    return _getUniqueFrom
      ( first ,
        last  ,
        typename Relations::RelationWeightedTypeTraits<FROM,TO,WEIGHT>::LessByFrom()  ,
        output ) ;
  }
  // ==========================================================================
  /** Trivial function to get all "FROM" elements form the sequnce
   *  @attention the sequence MUST be properly sorted
   *
   *  @code
   *
   *   Range links = ... ;
   *
   *   std::vector<FROM> res = ... ;
   *
   *   getUniqueFrom ( links ,
   *                   std::back_inserter ( res ) ) ;
   *
   *  @endcode
   *
   *  @param range  (INPUT)  the sequence
   *  @param output (OUTPUT) the outptu iterator
   *  @return advanced output iterator
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2009-05-24
   */
  template <class FROM, class TO, class OUTPUT>
  inline OUTPUT  getUniqueFrom
  ( const typename Relations::RelationTypeTraits<FROM,TO>::Range& range ,
    OUTPUT  output )
  {
    return _getUniqueFrom
      ( range.begin () ,
        range.end   () ,
        typename Relations::RelationTypeTraits<FROM,TO>::LessByFrom()  ,
        output ) ;
  }
  // ==========================================================================
  /** Trivial function to get all "FROM" elements form the sequnce
   *  @attention the sequence MUST be properly sorted
   *
   *  @code
   *
   *   Range links = ... ;
   *
   *   std::vector<FROM> res = ... ;
   *
   *   getUniqueFrom ( links ,
   *                   std::back_inserter ( res ) ) ;
   *
   *  @endcode
   *
   *  @param range  (INPUT)  the sequence
   *  @param output (OUTPUT) the outptu iterator
   *  @return advanced output iterator
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2009-05-24
   */
  template <class FROM, class TO, class WEIGHT, class OUTPUT>
  inline OUTPUT  getUniqueFrom
  ( const typename Relations::RelationTypeTraits<FROM,TO>::Range& range ,
    OUTPUT  output )
  {
    return _getUniqueFrom
      ( range.begin () ,
        range.end   () ,
        typename Relations::RelationWeightedTypeTraits<FROM,TO,WEIGHT>::LessByFrom()  ,
        output ) ;
  }
  // ==========================================================================
} // end of the namespace Relations
// ============================================================================
// The END
// ============================================================================
#endif // RELATIONS_GET_H
// ============================================================================
