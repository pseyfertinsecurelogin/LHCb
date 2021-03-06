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
#ifndef LOKI_DECAYFINDER_H
#define LOKI_DECAYFINDER_H
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <iterator>
#include <type_traits>
// ============================================================================
// GauduiKernel
// ============================================================================
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/Range.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#define LOKI_REQUIRES( ... ) std::enable_if_t<( __VA_ARGS__ ), bool> = true
// ============================================================================
/** @file LoKi/DecayFinder.h
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
namespace Decays {
  // ==========================================================================
  /** @class Finder LoKi/DecayFinder.h
   *
   *  Helper class to find/select the decays
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2009-06-02
   */
  template <class PARTICLE>
  class Finder_ {
  protected:
    // ========================================================================
    /// the type for collection
    typedef typename Decays::iTree_<PARTICLE>::Collection Collection;
    typedef typename Collection::const_iterator           iterator;
    // ========================================================================
  public:
    // ========================================================================
    typedef Decays::iTree_<PARTICLE> iTree;
    typedef Decays::Tree_<PARTICLE>  Tree;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the tree
    Finder_( const iTree_<PARTICLE>& tree ) : m_tree( tree ) {}
    // ========================================================================
  public:
    // ========================================================================
    /** check the presence of the decay in the input sequence
     *
     *  @code
     *
     *   SEQUENCE input = ... ;
     *
     *   const iTree_<PARTICLE>& tree = ... ;
     *
     *   // create the selector/fidner:
     *   Finder_<PARTICLE> finder (  tree ) ;
     *
     *   // use the selector
     *   const bool ok = finder.hasDecay( input.begin( ), input.end() ) ;
     *
     *  @endcode
     *
     *  @param first (INPUT) begin-iterator for the input sequence
     *  @param last  (INPUT) end-iterator   for the input sequence
     *  @return true if the specified decay is in the sequnce
     */
    template <typename ITERATOR,
              LOKI_REQUIRES( std::is_base_of<std::input_iterator_tag,
                                             typename std::iterator_traits<ITERATOR>::iterator_category>::value )>
    bool hasDecay( ITERATOR first, ITERATOR last ) const {
      return std::any_of( first, last, m_tree );
    }
    // ========================================================================
    bool hasDecay( const std::vector<PARTICLE>& particles ) const {
      return this->hasDecay( particles.begin(), particles.end() );
    }
    // ========================================================================
    bool hasDecay( const Gaudi::Range_<std::vector<PARTICLE>>& particles ) const {
      return this->hasDecay( particles.begin(), particles.end() );
    }
    // ========================================================================
    bool hasDecay( const Gaudi::NamedRange_<std::vector<PARTICLE>>& particles ) const {
      return this->hasDecay( particles.begin(), particles.end() );
    }
    // ========================================================================
  public:
    // ========================================================================
    /** find the specified decay in the input sequence
     *
     *  @code
     *
     *   SEQUENCE input = ... ;
     *
     *   const iTree_<PARTICLE>& tree = ... ;
     *
     *   // create the selectot:
     *   Finder_<PARTICLE> finder (  tree ) ;
     *
     *   std::vector<PARTICLE>  output ;
     *
     *   // use the selector
     *   finder.findDecay  ( input.begin( ), input.end() , output ) ;
     *
     *  @endcode
     *
     *  @param first  (INPUT)  begin-iterator for the input sequence
     *  @param last   (INPUT)  end-iterator   for the input sequence
     *  @param output (OUTPUT) vector of selected particles
     *  @return number of found particles
     */
    template <typename ITERATOR, typename OUTPUT,
              LOKI_REQUIRES( std::is_base_of<std::input_iterator_tag,
                                             typename std::iterator_traits<ITERATOR>::iterator_category>::value )>
    unsigned int findDecay( ITERATOR first, ITERATOR last, std::vector<OUTPUT>& output ) const {
      const unsigned int old_size = output.size();
      ( *this )( first, last, std::back_inserter( output ) );
      return output.size() - old_size;
    }
    // ========================================================================
    unsigned int findDecay( const std::vector<PARTICLE>& input, std::vector<PARTICLE>& output ) const {
      return this->findDecay( input.begin(), input.end(), output );
    }
    // ========================================================================
    unsigned int findDecay( const Gaudi::Range_<std::vector<PARTICLE>>& input, std::vector<PARTICLE>& output ) const {
      return this->findDecay( input.begin(), input.end(), output );
    }
    // ========================================================================
    unsigned int findDecay( const Gaudi::NamedRange_<std::vector<PARTICLE>>& input,
                            std::vector<PARTICLE>&                           output ) const {
      return this->findDecay( input.begin(), input.end(), output );
    }
    // ========================================================================
  public:
    // ========================================================================
    /** the major method for selection of particles from some sequence
     *
     *  @code
     *
     *   SEQUENCE input = ... ;
     *
     *   const iTree_<PARTICLE>& tree = ... ;
     *
     *   // create the selector/finder:
     *   Finder_<PARTICLE> finder (  tree ) ;
     *
     *   std::vector<PARTICLE>  output ;
     *
     *   // use the selector/finder:
     *   finder  ( input.begin( ), input.end() ,
     *             std::back_inserter ( output ) ) ;
     *
     *  @endcode
     *
     *  @param  first  (INPUT)  the begin  operator of the sequence
     *  @param  last   (INPUT)  the end    operator of the sequence
     *  @param  output (OUTPUT) the output iterator for the result
     *  @return advanced output operator
     */
    template <typename ITERATOR, typename OUTPUT,
              LOKI_REQUIRES( std::is_base_of<std::input_iterator_tag,
                                             typename std::iterator_traits<ITERATOR>::iterator_category>::value )>
    OUTPUT operator()( ITERATOR first, ITERATOR last, OUTPUT output ) const {
      const bool valid = m_tree.valid();
      if ( !valid ) { return output; } // No action for Invalid tree, RETURN
      const bool marked = m_tree.marked();
      Collection _mark;
      for ( ; first != last; ++first ) {
        if ( marked ) { m_tree.reset(); }
        // OK ?
        if ( !m_tree.eval( *first ) ) { continue; } // CONTINUE
        // the regular non-marked  descriptor
        if ( !marked ) {
          *output = *first; // assign
          ++output;         // advance
          continue;         // CONTINUE
        }
        // the descriptor with marked elements
        _mark.clear();
        m_tree.collect( _mark );
        for ( iterator imark = _mark.begin(); _mark.end() != imark; ++imark ) {
          *output = *imark; // assign
          ++output;         // advance
        }
        _mark.clear();
      }
      return output; // RETURN
    }
    // ========================================================================
    /** the major method for selection of particles from some sequence
     *
     *  @code
     *
     *   std:vector<PARTICLE> input = ... ;
     *
     *   const iTree_<PARTICLE>& tree = ... ;
     *
     *   // create the selector/finder:
     *   Finder_<PARTICLE> finder (  tree ) ;
     *
     *   std::vector<PARTICLE>  output ;
     *
     *   // use the selector
     *   finder  ( input , output ) ;
     *
     *  @endcode
     *
     *  @param  first  (INPUT)  the input  container
     *  @param  output (OUTPUT) the output container
     *  @return number of added elements
     */
    template <class INPUT, class OUTPUT>
    unsigned int operator()( const INPUT& input, std::vector<OUTPUT>& output ) const {
      const unsigned int old_size = output.size();
      // regirect to another method
      ( *this )( input.begin(), input.end(), std::back_inserter( output ) );
      return output.size() - old_size;
    }
    // ========================================================================
  public:
    // ========================================================================
    /// access to the underlying tree
    const iTree_<PARTICLE>& tree() const { return m_tree.tree(); }
    /// cast operator to the underlyiong tree
    operator const iTree_<PARTICLE>&() const { return this->tree(); } // cast
    /// valid tree?
    bool valid() const { return this->tree().valid(); }
    /// marked tree?
    bool marked() const { return this->tree().marked(); }
    /// invalid tree?
    bool operator!() const { return !( this->valid() ); }
    /// validate the tree
    StatusCode validate( const LHCb::IParticlePropertySvc* svc ) const { return m_tree.validate( svc ); }
    // ========================================================================
  private:
    // ========================================================================
    // no default constructor
    Finder_(); // no default constructor
    // ========================================================================
  private:
    // ========================================================================
    /// the actual selection tree
    Decays::Tree_<PARTICLE> m_tree; // the actual selection tree
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace Decays
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Utils {
    // ========================================================================
    template <class PARTICLE>
    std::string toCpp( const Decays::Finder_<PARTICLE>& p ) {
      return toCpp( p.tree() );
    }
    // ========================================================================
  } // namespace Utils
  // ==========================================================================
} // namespace Gaudi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_DECAYFINDER_H
