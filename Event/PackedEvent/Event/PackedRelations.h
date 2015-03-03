// $Id: $
#ifndef EVENT_PACKEDRELATIONS_H
#define EVENT_PACKEDRELATIONS_H 1

// Kernel
#include "Kernel/StandardPacker.h"

// Include files
namespace LHCb {


  class PackedRelation {
  public:
    PackedRelation() {}
    ~PackedRelation() {}

    int container;
    int start;
    int end;
  };
    
  /** @class PackedRelations PackedRelations.h Event/PackedRelations.h
   *
   *
   *  @author Olivier Callot
   *  @date   2012-01-24
   */

  static const CLID CLID_PackedRelations = 1560;

  /// Namespace for locations in TDS
  namespace PackedRelationsLocation {
    static const std::string& InStream = "/pPhys/Relations";
  }

  class PackedRelations : public DataObject {
  public:
    /// Standard constructor
    PackedRelations( ) {};

    virtual ~PackedRelations( ) {}; ///< Destructor

    /// Class ID
    static const CLID& classID() { return CLID_PackedRelations; }

    /// Class ID
    virtual const CLID& clID() const { return PackedRelations::classID(); }
    
    std::vector<PackedRelation>& relations() { return m_relations; }
    std::vector<int>& sources() { return m_source; }
    std::vector<int>& dests() { return m_dest; }

  protected:

  private:
    std::vector<PackedRelation> m_relations;
    std::vector<int> m_source;
    std::vector<int> m_dest;
  };
}
#endif // EVENT_PACKEDRELATIONS_H
