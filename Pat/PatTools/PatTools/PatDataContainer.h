// $Id: PatDataContainer.h,v 1.3 2006-06-01 13:43:25 ocallot Exp $
#ifndef PATTOOLS_PATDATACONTAINER_H 
#define PATTOOLS_PATDATACONTAINER_H 1

// Include files

/** @class PatDataContainer PatDataContainer.h PatTools/PatDataContainer.h
 *  Very simple container, not using TES.
 *
 *  @author Olivier Callot
 *  @date   2005-05-30
 */
template <class T > class PatDataContainer {
public: 
  /// Standard constructor
  PatDataContainer( int initialSize ) {
    for ( int kk = 0; initialSize > kk; ++kk ) {
      m_vector.push_back( new T() );
    }
    m_it     = m_vector.begin();
    m_maxSize = 0;
  }
  
  ~PatDataContainer( ) { ///< Destructor
    for ( unsigned int i=0; i < m_vector.size(); ++i) delete m_vector[i];
  }

  //! returns the actual used size of the filled vector
  int size() { return m_it - m_vector.begin(); }

  //! returns the maximal used size of the filled vector
  int maxSize() { return m_maxSize; }

  //! return a new element, the next empty element of the vector
  T* newEntry() {
    if (m_it == m_vector.end()) {
      m_vector.push_back( new T( ) );
      m_it = m_vector.end();
      m_it--;
    }
    return *m_it++;
    // for "placement new", use the next statement, but quite slower !
    //return new(*m_it++) T();
  }

  //! returns the first element of the iterator
  typename std::vector<T*>::const_iterator begin() { return m_vector.begin();  }

  //! returns the last element of the iterator
  typename std::vector<T*>::const_iterator end()   { return m_it; }

  //! reset the filled vector
  void initEvent() {
    updateSize();
    m_it   = m_vector.begin();
  }
  
  //! update Size
  void updateSize() {
    int curSize = size();
    if ( m_maxSize < curSize ) m_maxSize = curSize;
  }
  
protected:

private:
  std::vector<T*>   m_vector;
  int               m_maxSize;
  typename std::vector<T*>::const_iterator m_it;
};
#endif // PATTOOLS_PATDATACONTAINER_H
