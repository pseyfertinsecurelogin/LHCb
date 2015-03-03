// $Id: PatContainer.h,v 1.7 2007-08-27 14:06:11 mneedham Exp $
#ifndef PATTOOLS_PATCONTAINER_H 
#define PATTOOLS_PATCONTAINER_H 1

// Include files
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/KeyedContainer.h"

/** @class PatContainer PatContainer.h PatTools/PatContainer.h
 *  A template class to interface to a 'vector of pointers' to
 *  objects. It may be a simple interface to the TES use, or it handles
 *  space allocation and re-use using placement new whenever needed.
 *
 *  @author Olivier Callot
 *  @date   2005-05-20
 */
template <class DATATYPE > class PatContainer {
public:

  /// Definition of the contained object type
  typedef DATATYPE                                  contained_type;
  /// Definition of the STL sequential access type
  typedef typename std::vector<contained_type*>     seq_type;
  /// Sequential access: reference type used in sequential container
  typedef typename seq_type::reference              reference;
  /// Sequential access: const reference type used in sequential container
  typedef typename seq_type::const_reference        const_reference;
  /// Sequential access: iterator type used in sequential container
  typedef typename seq_type::iterator               iterator;
  /// Sequential access: const iterator type used in sequential container
  typedef typename seq_type::const_iterator         const_iterator;

  /// Standard constructor
  PatContainer( const std::string& name, int initialSize,
                bool onTES, IDataProviderSvc* evtSvc ) {
    m_name   = name;
    m_onTES  = onTES;
    m_evtSvc = evtSvc;
    if ( !onTES ) {   // Create enough usable objects.
      for ( int kk = 0; initialSize > kk; ++kk ) {
        m_vector.push_back( new contained_type() );
      }
    }
    m_it     = m_vector.begin();
    m_maxSize = 0;
  }

  //! delete the vector: delete also its contents, if not on TES.
  ~PatContainer() {
    if ( !m_onTES ) {
      for ( unsigned int i=0; i < m_vector.size(); ++i) delete m_vector[i];
    }    
  }

  //! returns the actual used size of the filled vector
  int size() { return m_it - m_vector.begin(); }

  //! returns the maximal used size of the filled vector
  int maxSize() { return m_maxSize; }

  //! returns the name of the container
  std::string& name()  const { return m_name; }

  //! return a new element, the next empty element of the vector
  contained_type* newEntry() {    // Automatic key = size of vector.
    return  newEntry( size() );
  }
  
  //! return a new element with key, the next element of the vector
  contained_type* newEntry( int key ) {
    if ( m_onTES ) {
      // Create an object, and store it in the container.
      if (m_it == m_vector.begin()) retrieveKeyedContainer();
      contained_type* temp = new contained_type( key );
      m_container->insert( temp );
      m_vector.push_back( temp );
      m_it = m_vector.end();
      return temp;
    }
    
    if (m_it == m_vector.end()) {
      m_vector.push_back( new contained_type( key ) ); // Set the key also...
      m_it = m_vector.end();
      m_it--;
    }
    contained_type* temp = new(*m_it) contained_type( key );
    m_it++;
    return temp;
  }

  //! returns the first element of the iterator
  const_iterator begin() const { return m_vector.begin();  }
  
  //! returns the last element of the iterator
  const_iterator end()   const { return m_it; }
  
  //! returns the first element of the iterator
  iterator begin() { return m_vector.begin();}

  //! returns the last element of the iterator
  iterator end() { return m_it; }

  //! reset the filled vector
  void initEvent(  ) {
    updateSize();
    m_it   = m_vector.begin();
    if (m_onTES) {
      m_vector.clear();
      m_it   = m_vector.begin();
      retrieveKeyedContainer();    
    }

  }

  //! Update size
  void updateSize(  ) {
    int curSize = size();
    if ( m_maxSize < curSize ) m_maxSize = curSize;
  }

  //! update index : Re-compute the internal table form the TES container
  void updateTable() {
    if ( !m_onTES ) return;
    m_vector.clear();
    typedef typename KeyedContainer<contained_type>::iterator Kiter;
    for ( Kiter it  = m_container->begin(); m_container->end() != it; ++it) {
      m_vector.push_back( *it);
    }
    m_it = m_vector.end();
  }
  
protected:

  void retrieveKeyedContainer() {
    m_container = SmartDataPtr<KeyedContainer<contained_type> >( m_evtSvc,m_name );
    if ( NULL == m_container ) {
      m_container = new KeyedContainer<contained_type>();
      m_evtSvc->registerObject( m_name, m_container );
    }
  }
  

private:
  bool               m_onTES;
  std::string        m_name;
  seq_type    m_vector;
  iterator m_it;
  KeyedContainer<contained_type>* m_container;
  IDataProviderSvc*  m_evtSvc;
  int                m_maxSize;
};
#endif // PATTOOLS_PATCONTAINER_H
