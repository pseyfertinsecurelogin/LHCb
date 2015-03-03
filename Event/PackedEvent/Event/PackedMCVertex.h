// $Id: PackedMCVertex.h,v 1.2 2006-02-16 15:21:15 cattanem Exp $
#ifndef EVENT_PACKEDMCVERTEX_H 
#define EVENT_PACKEDMCVERTEX_H 1

// Include files
#include "GaudiKernel/DataObject.h"
#include <vector>

/** @class PackedMCVertices PackedMCVertex.h Event/PackedMCVertex.h
 *  DataObject containing a vector of packed MCVertices
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
namespace LHCb 
{
  struct PackedMCVertex {
    int key;
    int x;
    int y;
    int z;
    float tof;
    int type;
    int mother;
    std::vector<int> products;
  };

  static const CLID& CLID_PackedMCVertices = 1511;

  // Namespace for locations in TDS
  namespace PackedMCVertexLocation {
    static const std::string& Default = "pSim/MCVertices";
  }
  
  class PackedMCVertices : public DataObject {
  public: 
    /// Standard constructor
    PackedMCVertices( ) {}; 

    virtual ~PackedMCVertices( ) {}; ///< Destructor
    virtual const CLID& clID() const { return PackedMCVertices::classID(); }
    static  const CLID& classID()    { return CLID_PackedMCVertices;       }
    
    void addEntry(  PackedMCVertex& obj ) { m_vect.push_back( obj ); }
    std::vector<PackedMCVertex>::const_iterator begin() const { return m_vect.begin(); }
    std::vector<PackedMCVertex>::const_iterator end()   const { return m_vect.end(); }

  protected:

  private:
    std::vector<PackedMCVertex> m_vect;
  };
} // namespace LHCb

#endif // EVENT_PACKEDMCVERTEX_H
