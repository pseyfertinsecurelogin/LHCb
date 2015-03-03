// $Id: Decays.cpp,v 1.3 2008-12-05 09:09:21 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Decays.h"
#include "LoKi/Sections.h"
#include "LoKi/Child.h"
#include "LoKi/PhysAlgs.h"
#include "LoKi/Algs.h"
// ============================================================================
/** @file
 *  Implementation file for LoKi MC-tree-functors 
 *  @author Vanya BELYAEV Ivan.BELYAEV@nikhef.nl
 *  @date   2008-05-25
 */
// ============================================================================

namespace 
{
  // ==========================================================================
  /** make the decay sections 
   *  @author Vanya BELYAEV Ivan.BELYAEV@nikhef.nl
   *  @date   2008-05-25
   */
  inline void makeSections 
  ( const LHCb::Particle*          p     , 
    const Decays::Trees::Alg alg   , 
    Decays::Sections&        sect  ) 
  {
    if ( 0 == p ) { return ; }
    //
    if ( Decays::Trees::Sections == alg ) 
    { Decays::sections ( p , sect ) ; }
    else 
    {
      Decays::Section s ;
      Decays::section ( p , s ) ;      
      sect.push_back ( s ) ;    
    }
  }
  // ==========================================================================
  struct NotPid : public std::unary_function<const LHCb::Particle*,bool>
  {
    /// constructor 
    NotPid ( const Decays::Nodes::Pid& pid ) : m_pid ( pid ) {}
    // the major method: find "not a pid" 
    inline bool operator () ( const LHCb::Particle* p ) const 
    { return 0 != p && !m_pid.check ( p->particleID() ) ; }
  private:
    // ========================================================================
    /// the pid-checker 
    Decays::Nodes::Pid m_pid ; //the pid-checker 
    // ========================================================================    
  } ;
  // ==========================================================================
  /** scan the sequence of particles to check that it containes ONLY 
   *  the partiles of the given PID 
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-05-26
   */
  template <class TYPE>
  inline bool onlyPid 
  (  TYPE begin , 
     TYPE end   , 
     const Decays::Nodes::Pid& pid ) 
  {
    return end == std::find_if ( begin , end , NotPid ( pid ) ) ; 
  }
  // ==========================================================================
  /// the helper object tfor comparison 
  typedef Decays::Trees::CheckTree<const LHCb::Particle*> Equal ;  
  // ==========================================================================
}
// ============================================================================
// EXCLUSIVE 
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const Decays::iNode&                      mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      )
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( mother     )
  , m_children   ( children   ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const Decays::iNode&                        mother    , 
  const Decays::Trees::Alg                    alg       ) 
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( mother     )
  , m_children   (            ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const std::string&                              mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      )
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( Decays::Nodes::Pid ( mother ) ) 
  , m_children   ( children   ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const std::string&                                mother    , 
  const Decays::Trees::Alg                    alg       ) 
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( Decays::Nodes::Pid ( mother ) ) 
  , m_children   (            ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const LHCb::ParticleID&                         mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      )
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( Decays::Nodes::Pid ( mother ) ) 
  , m_children   ( children   ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const LHCb::ParticleID&                           mother    , 
  const Decays::Trees::Alg                    alg       ) 
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( Decays::Nodes::Pid ( mother ) ) 
  , m_children   (            ) 
  , m_alg        ( alg        )
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Exclusive::Exclusive
( const Decays::Decay&                                decay     , 
  const Decays::Trees::Alg                    alg       ) 
  : Decays::iTree_<const LHCb::Particle*> () 
  , m_mother     ( Decays::Nodes::Pid ( decay.mother() ) )
  , m_children   (            ) 
  , m_alg        ( alg        )
{ 
  setChildren ( decay.children() ) ; 
}
// ============================================================================
// set the children 
// ============================================================================
void Decays::Trees::Exclusive::setChildren 
( const std::vector<std::string>& children ) 
{
  m_children.clear() ;
  for ( std::vector<std::string>::const_iterator ichild = children.begin() ;
        children.end() != ichild ; ++ichild ) { addDaughter ( *ichild ) ; }
}
// ============================================================================
// set the children 
// ============================================================================
void Decays::Trees::Exclusive::setChildren 
( const std::vector<Decays::Decay::Item>& children ) 
{
  m_children.clear() ;
  for ( std::vector<Decays::Decay::Item>::const_iterator ichild = children.begin() ;
        children.end() != ichild ; ++ichild ) { addDaughter ( *ichild ) ; }
}
// ============================================================================
// set the children 
// ============================================================================
void Decays::Trees::Exclusive::setChildren 
( const std::vector<LHCb::ParticleID>& children ) 
{
  m_children.clear() ;
  for ( std::vector<LHCb::ParticleID>::const_iterator ichild = children.begin() ;
        children.end() != ichild ; ++ichild ) { addDaughter ( *ichild ) ; }
}
// ============================================================================
// set the children 
// ============================================================================
void Decays::Trees::Exclusive::setChildren 
( const std::vector<const LHCb::ParticleProperty*>& children ) 
{
  m_children.clear() ;
  for ( std::vector<const LHCb::ParticleProperty*>::const_iterator ichild = 
          children.begin() ; children.end() != ichild ; ++ichild ) 
  { addDaughter ( *ichild ) ; }
}



// ============================================================================
// MANDATORY: check the validity 
// ============================================================================
bool Decays::Trees::Exclusive::valid() const 
{
  return m_mother.valid() && Decays::valid ( childBegin() , childEnd() ) ;
}
// ============================================================================
// MANDATORY: validate the tree 
// ============================================================================
StatusCode  Decays::Trees::Exclusive::validate 
( const LHCb::IParticlePropertySvc* svc ) const 
{
  StatusCode sc = m_mother.validate ( svc ) ;
  if ( sc.isFailure() ) { return sc ; }
  return Decays::validate ( childBegin() , childEnd() , svc ) ;
}
// ============================================================================
// reset the collection cache
// ============================================================================
void Decays::Trees::Exclusive::reset() const { i_reset () ; }
// ============================================================================
// collect the marked elements 
// ============================================================================
size_t Decays::Trees::Exclusive::collect 
( Decays::iTree_<const LHCb::Particle*>::Collection& output ) const 
{
  size_t size = 0 ;
  for ( SubTrees::const_iterator ichild = childBegin() ; 
        childEnd() != ichild ; ++ichild ) { size += ichild->collect ( output ) ; }  
  return size ;
}
// ============================================================================
// the only one essential method 
// ============================================================================
bool Decays::Trees::Exclusive::operator() 
  ( Decays::iTree_<const LHCb::Particle*>::argument p ) const
{
  // reset the cache (ALWAYS!!!)
  i_reset () ;
  // check the validness and oscillation criteria 
  if ( !ok ( p )                      ) { return false ; }        // RETURN 
  // check the mother node
  if ( !mother ( p -> particleID () ) ) { return false ; }        // RETURN 
  // perform the real matching:
  if ( 0 == nChildren ()              ) { return true  ; }        // RETURN    
  // (1) get the proper decay sections:
  Decays::Sections sections ;
  makeSections ( p , alg() , sections ) ;
  // (2) loop over all sections
  for (  Decays::Sections::iterator isect = sections.begin() ; 
         sections.end() != isect ; ++isect )
  {
    // (3) try to match the section 
    // skip the combinations  which does not match at all 
    if ( nChildren() != isect->size() ) { continue ; }
    // (4) sort the section 
    std::stable_sort ( isect->begin() , isect->end() ) ;
    // make all possible permutations of the section  
    do  // make all possible permutations of the section  
    {
      // (5) match all fields:
      if ( std::equal ( childBegin() , childEnd() , isect->begin() , Equal() ) ) 
      { return true ; }                                            // RETURN 
    } // next permutation
    while ( std::next_permutation ( isect->begin() , isect->end() ) ) ;
  } // next section
  // no match 
  return false ;                                                   // RETURN 
}
// ============================================================================
// MANDATORY: the specific printout 
// ============================================================================
std::ostream& 
Decays::Trees::Exclusive::fillStream ( std::ostream& s ) const 
{
  //
  if ( 0 == nChildren() ) { return s << mother() ; }
  //
  s << " (" << mother() ;
  //
  switch ( this->alg() ) 
  {
  case Sections : s << "-->" ; break ;
  default       : s << " ->" ; break ;
  }
  //
  for ( SubTrees::const_iterator itree = childBegin() ; 
        childEnd() != itree ; ++itree ) { s << ( *itree ) ; }
  //
  return s << ") " ;
} 
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const Decays::iTree_<const LHCb::Particle*>& tree ) 
{ m_children.push_back ( tree ) ; }
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const Decays::iNode& node ){ addDaughter ( Exclusive ( node ) ) ; }
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const Decays::Decay::Item& node )
{ addDaughter ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const std::string&       node )
{ addDaughter ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const LHCb::ParticleID&  node )
{ addDaughter ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more node to the tree 
// ============================================================================
void Decays::Trees::Exclusive::addDaughter 
( const LHCb::ParticleProperty*  node )
{ addDaughter ( Decays::Nodes::Pid ( node ) ) ; }



// ============================================================================
// INCLUSIVE 
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const Decays::iNode&                      mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      ) 
  : Decays::Trees::Exclusive ( mother , children , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const Decays::iNode&                        mother , 
  const Decays::Trees::Alg                    alg    ) 
  : Decays::Trees::Exclusive( mother , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const std::string&                              mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      ) 
  : Decays::Trees::Exclusive ( mother , children , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const std::string&                                mother , 
  const Decays::Trees::Alg                    alg    ) 
  : Decays::Trees::Exclusive( mother , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param children the list of daughter substrees 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const LHCb::ParticleID&                         mother   , 
  const Decays::Trees::Exclusive::SubTrees& children ,
  const Decays::Trees::Alg                  alg      ) 
  : Decays::Trees::Exclusive ( mother , children , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const LHCb::ParticleID&                           mother , 
  const Decays::Trees::Alg                    alg    ) 
  : Decays::Trees::Exclusive( mother , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const Decays::Decay&                                decay  , 
  const Decays::Trees::Alg                    alg    ) 
  : Decays::Trees::Exclusive( decay  , alg ) 
{}
// ============================================================================
/*  constructor from the node (mother), subtrees and "final" flag
 *  @param mother the mother node 
 *  @param alg the matching algorithm 
 */
// ============================================================================
Decays::Trees::Inclusive::Inclusive
( const Decays::Trees::Exclusive& right ) 
  : Decays::Trees::Exclusive ( right ) 
{}
// ============================================================================




// ============================================================================
// MANDATORY: check the validity of the tree 
// ============================================================================
bool Decays::Trees::Inclusive::valid() const 
{ return Exclusive::valid() && 0 != nChildren() ; }
// ============================================================================
// the only one essential method 
// ============================================================================
bool Decays::Trees::Inclusive::operator() 
  ( Decays::iTree_<const LHCb::Particle*>::argument p ) const
{
  // reset the cache (ALWAYS!!!)
  i_reset () ;
  // check the validness and oscillation criteria 
  if ( !ok ( p )                      ) { return false ; }        // RETURN 
  // check the mother node
  
  // (0) treat specifically the special case 
  
  // Only one inclusive particle and look through the whole tree
  if ( 1 == nChildren () )
  {
    LHCb::Particle::ConstVector d ;
    LoKi::Child::daughters ( p , d ) ;
    // in tree 
    if ( Decays::Trees::Sections == alg() ) 
    {
      return LoKi::PhysAlgs::found ( d.begin() , d.end() , *childBegin() ) ;
    }
    // direct daughters :
    return d.end () != std::find_if ( d.begin() , d.end() , *childBegin() ) ;
  }
  
  // (1) get the proper decay sections:
  Decays::Sections sections ;
  makeSections ( p , alg() , sections ) ;
  
  // (2) loop over all sections
  for (  Decays::Sections::iterator isect = sections.begin() ; 
         sections.end() != isect ; ++isect )
  {
    // (3) try to match the section 
    // skip the combinations  which does not match at all 
    if ( nChildren() > isect->size() ) { continue ; }
    
    // the special case of 2 elements  
    if      ( 2 == nChildren () ) 
    {
      if ( LoKi::Algs::found_2 
           ( isect -> begin () , 
             isect -> end   () , 
             front () , back () ) ) { return true ; }          // RETURN 
      continue ;                                               // CONTINUE 
    }
    else 
    {
      if ( LoKi::Algs::found_N 
           ( isect->begin  () , 
             isect->end    () , 
             children ()      ) ) { return true ; }            // RETURN 
      continue ;                                               // CONTINUE ;  
    }
    
    // never go further, unfortunately the most generic algorithm
    // it terribly slow... O(N!)
    
    // (4) sort the section 
    std::stable_sort ( isect->begin() , isect->end() ) ;
    // make all possible permutations of the section  
    do 
    {
      // (5) match all fields:
      if ( std::equal ( childBegin() , childEnd() , isect->begin() , Equal() ) ) 
      { return true ; }                                            // RETURN 
    } // next permutation
    while ( std::next_permutation ( isect->begin() , isect->end() ) ) ;
  } // next section
  // no match 
  return false ;                                                   // RETURN 
}
// ============================================================================
// MANDATORY: the specific printout 
// ============================================================================
std::ostream& 
Decays::Trees::Inclusive::fillStream ( std::ostream& s ) const 
{
  //
  s << " (" << mother () ;
  //
  switch ( this->alg() ) 
  {
  case Sections : s << "-->" ; break ;
  default       : s << " ->" ; break ;
  }
  //
  for ( SubTrees::const_iterator itree = childBegin() ; 
        childEnd() != itree ; ++itree ) { s << ( *itree ) ; }
  //
  return s << " ... ) " ;
}
// ============================================================================


// ============================================================================
// OPTIONAL
// ============================================================================
Decays::Trees::Optional::Optional
( const Decays::iNode&                      mother     , 
  const Decays::Trees::Exclusive::SubTrees& children   ,
  const Decays::Trees::Exclusive::SubTrees& optional   ,
  const Decays::Trees::Alg                  alg        ) 
  : Decays::Trees::Exclusive ( mother , children , alg )
  , m_optional ( optional ) 
{}
// ============================================================================
Decays::Trees::Optional::Optional
( const std::string&                              mother     , 
  const Decays::Trees::Exclusive::SubTrees& children   ,
  const Decays::Trees::Exclusive::SubTrees& optional   ,
  const Decays::Trees::Alg                  alg        ) 
  : Decays::Trees::Exclusive ( mother , children , alg )
  , m_optional ( optional ) 
{}
// ============================================================================
Decays::Trees::Optional::Optional
( const LHCb::ParticleID&                         mother     , 
  const Decays::Trees::Exclusive::SubTrees& children   ,
  const Decays::Trees::Exclusive::SubTrees& optional   ,
  const Decays::Trees::Alg                  alg        ) 
  : Decays::Trees::Exclusive ( mother , children , alg )
  , m_optional ( optional ) 
{}
// ============================================================================
Decays::Trees::Optional::Optional
( const Decays::Decay&                              decay    , 
  const Decays::Trees::Exclusive::SubTrees& optional ,
  const Decays::Trees::Alg                  alg      ) 
  : Decays::Trees::Exclusive ( decay , alg ) 
  , m_optional ( optional ) 
{}
// ============================================================================
Decays::Trees::Optional::Optional
( const Decays::Trees::Optional::Exclusive& right    , 
  const Decays::Trees::Exclusive::SubTrees& optional ) 
  : Decays::Trees::Exclusive ( right ) 
  , m_optional ( optional )  
{}
// ============================================================================
// MANDATORY: check the validity of the tree 
// ============================================================================
bool Decays::Trees::Optional::valid() const 
{
  return Exclusive::valid() && 0 != nChildren() && 0 != nOptional() && 
    Decays::valid ( optBegin( ), optEnd() ) ;
}
// ============================================================================
// MANDATORY: the proper validation of the tree
// ============================================================================
StatusCode Decays::Trees::Optional::validate 
( const LHCb::IParticlePropertySvc* svc ) const 
{ 
  StatusCode sc = Exclusive::validate ( svc ) ;
  return sc.isFailure() ? sc :
    Decays::validate ( optBegin() , optEnd() , svc ) ;
}
// ============================================================================
// MANDATORY: the specific printout 
// ============================================================================
std::ostream& Decays::Trees::Optional::fillStream 
( std::ostream& s ) const 
{
  //
  s << " (" << mother () ;
  //
  switch ( this->alg() ) 
  {
  case Sections : s << "-->" ; break ;
  default       : s << " ->" ; break ;
  }
  //
  for ( SubTrees::const_iterator itree = childBegin() ;
        childEnd() != itree ; ++itree )  { s <<         ( *itree )         ; }
  for ( SubTrees::const_iterator itree = optBegin() ; 
        optEnd () != itree ; ++itree )   { s << " {" << ( *itree ) << "} " ; }
  //
  return s << ") " ;
} 
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool Decays::Trees::Optional::operator() 
  ( Decays::iTree_<const LHCb::Particle*>::argument p ) const 
{
  // reset the cache (ALWAYS!!!)
  i_reset () ;
  // check the validness and oscillation criteria 
  if ( !ok ( p )                       ) { return false ; }        // RETURN 
  // perform the real matching:
  // (1) get the proper decay sections:
  Decays::Sections sections ;
  makeSections ( p , alg() , sections ) ;
  // (2) loop over all sections
  for (  Decays::Sections::iterator isect = sections.begin() ; 
         sections.end() != isect ; ++isect )
  {
    // (3) try to match the section 
    // skip the combinations  which does not match at all 
    if ( isect->size() < nChildren ()                ) { continue ; } // CONITNUE
    if ( isect->size() > nChildren () + nOptional () ) { continue ; } // CONTINUE
    // (4) sort the section 
    std::stable_sort ( isect->begin() , isect->end() ) ;
    // (5) check "inclusive"
    if ( !LoKi::Algs::found_N 
         ( isect->begin (), 
           isect->end   (), 
           children() )   ) { continue ; }                        // CONTINUE 
    // (6) make all possible permutations:
    do // loop over all permutations 
    {
      // (7) match all declared mandatory fields:
      if ( std::equal ( childBegin() , childEnd() , isect->begin() , Equal () ) )
      {
        // the temporary iterator 
        Decays::Section::iterator aux = isect->begin() + nChildren () ;
        // create the local container for permutations 
        SubTrees opt ( m_optional ) ;
        std::stable_sort ( opt.begin () , opt.end() ) ; // sort it!!!
        // start all possible permutations of the optional stuff 
        do  
        {
          if ( std::equal ( aux , isect->end() , opt.begin() , Equal () ) ) 
          { return true ; }                                       // RETURN  
        } // the next permutation for the optional stuff 
        while ( std::next_permutation ( opt.begin() , opt.end () ) ) ;
      } // match mandatory stuff 
    } // next permutation
    while ( std::next_permutation ( isect->begin() , isect->end() ) ) ;  
  } // next section
  // no match 
  return false ; // RETURN 
}
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const Decays::iTree_<const LHCb::Particle*>& tree ) 
{ m_optional.push_back ( tree ) ; }
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const Decays::iNode& node ) 
{ m_optional.push_back ( Exclusive ( node ) ) ; }
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const std::string& node ) 
{ addOptional ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const LHCb::ParticleID& node ) 
{ addOptional ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const Decays::Decay::Item& node ) 
{ addOptional ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// add one more daughter to the decay 
// ============================================================================
void Decays::Trees::Optional::addOptional 
( const LHCb::ParticleProperty* node ) 
{ addOptional ( Decays::Nodes::Pid ( node ) ) ; }
// ============================================================================
// set optional
// ============================================================================
void Decays::Trees::Optional::setOptional
( const std::vector<const LHCb::ParticleProperty*>& optional ) 
{
  m_optional.clear() ;
  for ( std::vector<const LHCb::ParticleProperty*>::const_iterator iopt = 
          optional.begin() ; optional.end() != iopt; ++iopt ) 
  { addOptional ( *iopt ) ; }  
}
// ============================================================================
// set optional
// ============================================================================
void Decays::Trees::Optional::setOptional
( const std::vector<LHCb::ParticleID>& optional ) 
{
  m_optional.clear() ;
  for ( std::vector<LHCb::ParticleID>::const_iterator iopt = 
          optional.begin() ; optional.end() != iopt; ++iopt ) 
  { addOptional ( *iopt ) ; }  
}
// ============================================================================
// set optional
// ============================================================================
void Decays::Trees::Optional::setOptional
( const std::vector<Decays::Decay::Item>& optional ) 
{
  m_optional.clear() ;
  for ( std::vector<Decays::Decay::Item>::const_iterator iopt = 
          optional.begin() ; optional.end() != iopt; ++iopt ) 
  { addOptional ( *iopt ) ; }  
}
// ============================================================================
// set optional
// ============================================================================
void Decays::Trees::Optional::setOptional
( const std::vector<std::string>& optional ) 
{
  m_optional.clear() ;
  for ( std::vector<std::string>::const_iterator iopt = 
          optional.begin() ; optional.end() != iopt; ++iopt ) 
  { addOptional ( *iopt ) ; }  
}
// ============================================================================




// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::And_<const LHCb::Particle*>
operator&& 
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const Decays::iNode&                         n ) 
{ return t && Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::And_<const LHCb::Particle*>
operator&& 
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const Decays::Decay::Item&                           n ) 
{ return t && Decays::Trees::Exclusive 
    ( Decays::Nodes::Pid ( n ) ) ;  }
// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::And_<const LHCb::Particle*>
operator&& 
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const std::string&                                 n ) 
{ return t && Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================
/*  operator for the easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::And_<const LHCb::Particle*>
operator&& 
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const LHCb::ParticleID&                            n ) 
{ return t && Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::Or_<const LHCb::Particle*>
operator|| 
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const Decays::iNode&                         n ) 
{ return t || Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::Or_<const LHCb::Particle*>
operator||
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const Decays::Decay::Item&                           n ) 
{ return t || Decays::Trees::Exclusive 
    ( Decays::Nodes::Pid ( n ) ) ;  }
// ============================================================================
/*  operator for easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::Or_<const LHCb::Particle*>
operator||
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const std::string&                                 n ) 
{ return t || Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================
/*  operator for the easy construction of trees 
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-07
 */
// ============================================================================
Decays::Trees::Or_<const LHCb::Particle*>
operator||
( const Decays::iTree_<const LHCb::Particle*>& t , 
  const LHCb::ParticleID&                            n ) 
{ return t || Decays::Trees::Exclusive ( n ) ;  }
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
