// $Id: PatContainerMap.h,v 1.3 2006-06-01 13:43:25 ocallot Exp $
#ifndef PATTOOLS_PATCONTAINERMAP_H 
#define PATTOOLS_PATCONTAINERMAP_H 1

// Include files
#include "PatTools/PatContainer.h"

/** @class PatContainerMap PatContainerMap.h PatTools/PatContainerMap.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-05-26
 */
template <class T > class PatContainerMap {
public:
  typedef typename std::map< std::string, PatContainer<T>* >::iterator iterator;

  /// Standard constructor
  PatContainerMap() {};

  virtual ~PatContainerMap( ) { ///< Destructor
    typename std::map<std::string,PatContainer<T>*>::const_iterator it;
    for ( it = m_map.begin(); it != m_map.end(); it++) {
      delete it->second;
    }
  }

  //! create and return a container of PatVertexs
  PatContainer<T>* createContainer( const std::string& myName, 
                                    int  nsize,
                                    bool onTES,
                                    IDataProviderSvc* evtSvc) {
    if ( m_map.find( myName ) == m_map.end() ) {
      m_map[myName] = new PatContainer<T>( myName, nsize, onTES, evtSvc );
    }
    return m_map[myName];
  }

  //! access on PatContainer by name
  PatContainer<T>* container( const std::string& myName ) {
    if ( m_map.find( myName ) == m_map.end() ) return NULL;
    return m_map[myName];
  }

  //! initEvent for all containers
  void initEvent( ) {
    typename std::map<std::string,PatContainer<T>*>::const_iterator it;
    for ( it = m_map.begin(); m_map.end() != it; ++it ) {
      (*it).second->initEvent( );
    }
  }

  //! updateSize of all containers
  void updateSize( ) {
    typename std::map<std::string,PatContainer<T>*>::const_iterator it;
    for ( it = m_map.begin(); m_map.end() != it; ++it ) {
      (*it).second->updateSize( );
    }
  }

  //! begin and end iterators
  iterator begin()  { return m_map.begin(); }

  iterator end()    { return m_map.end(); }

protected:

private:
  std::map< std::string, PatContainer<T>* > m_map;
  
};
#endif // PATTOOLS_PATCONTAINERMAP_H
