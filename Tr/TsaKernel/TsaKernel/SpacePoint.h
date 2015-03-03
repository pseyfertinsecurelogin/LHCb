#ifndef _TsaSpacePoint_H
#define _TsaSpacePoint_H 1

/** @class TsaSpacePoint TsaSpacePoint.h
*
*  spacepoint class for Tsa
*
*  @author Matthew Needham
*  created Wed Jun 25 14:16:12 2003
*
*/

// STL
#include <algorithm>
#include <vector>

// Gaudi
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/Point3DTypes.h"

#include "TsaKernel/Cluster.h"

#include "GaudiKernel/boost_allocator.h"


namespace Tsa{

  namespace SpacePointLocation {
    static const std::string& Default = "/Event/Rec/Tsa/SpacePoints";
  }

  // Class ID definition
  static const CLID& CLID_SpacePoint = 19104;

  class SpacePoint: public KeyedObject<int>{

    public:

      /// clone
      SpacePoint* clone() const;

      /// enum for type
      enum pointType {Unknown = 0, R, L, B , T , OB, OT};

      /// constructer
      SpacePoint(const Gaudi::XYZPoint& aPoint, 
                 double tX, double tY, 
	         Tsa::SpacePoint::pointType aType,
	         unsigned int station,
	         std::vector<Tsa::Cluster*>& clusCont);

      /// Destructor
      virtual ~SpacePoint(){}

      virtual const CLID& clID() const;
      static const CLID& classID();

      /// short cut to x
      double x() const;

      /// short cut to y
      double y() const;

      /// short cut to z
      double z() const;

      /// Retrieve mewsured point (mm)
      const Gaudi::XYZPoint& point() const;

      /// Retrieve measured x slope val
      double tX() const;

      /// measured y slope val
      double tY() const;

      /// Retrieve smartref of clusters (const)
      const std::vector<Tsa::Cluster*>& clusters() const;

      /// Add a cluster
      void addToClusters(Tsa::Cluster* value);

      /// Retrieve smartref of clusters (non-const)
      //std::vector<Tsa::Cluster*>& clusters();

      /// Remove smartref of clusters 
      void removeFromClusters(const Tsa::Cluster* value);

      /// short cut to station
      unsigned int station() const;    

      /// short cut to type
      pointType type() const;

      /// size
      unsigned int size() const;

      /// it or ot
      bool isIT() const;

      /// used count
      unsigned int pass() const;

      /// update
      void updatePass(const unsigned int pass);  

#ifndef _WIN32
    /// operator new
    static void* operator new ( size_t size )
    {
      return ( sizeof(SpacePoint) == size ?
               boost::singleton_pool<SpacePoint, sizeof(SpacePoint)>::malloc() :
               ::operator new(size) );
    }

    /// placement operator new
    /// it is needed by libstdc++ 3.2.3 (e.g. in std::vector)
    /// it is not needed in libstdc++ >= 3.4
    static void* operator new ( size_t size, void* pObj )
    {
      return ::operator new (size,pObj);
    }

    /// operator delete
    static void operator delete ( void* p )
    {
      boost::singleton_pool<SpacePoint, sizeof(SpacePoint)>::is_from(p) ?
      boost::singleton_pool<SpacePoint, sizeof(SpacePoint)>::free(p) :
      ::operator delete(p);
    }

    /// placement operator delete
    /// not sure if really needed, but it does not harm
    static void operator delete ( void* p, void* pObj )
    {
      ::operator delete (p, pObj);
    }
#endif


    private:

     SpacePoint(const SpacePoint& point);

     Gaudi::XYZPoint m_point;
     double m_tX;
     double m_tY;
     pointType m_type;
     unsigned int m_station;
     std::vector<Tsa::Cluster*> m_clusters;
     unsigned int m_pass;
  };
};

inline const CLID& Tsa::SpacePoint::clID() const{
  return Tsa::SpacePoint::classID();
}

inline const CLID& Tsa::SpacePoint::classID(){
  return CLID_SpacePoint;
}

inline double Tsa::SpacePoint::x() const{
  return m_point.x();
}

inline double Tsa::SpacePoint::y() const{
  return m_point.y();
}

inline double Tsa::SpacePoint::z() const{
  return m_point.z();
}

inline const Gaudi::XYZPoint& Tsa::SpacePoint::point() const{
  return m_point;
}


inline double Tsa::SpacePoint::tX() const{
  return m_tX;
}

inline double Tsa::SpacePoint::tY() const{
  return m_tY;
}

inline const std::vector<Tsa::Cluster*>& Tsa::SpacePoint::clusters() const{
   return m_clusters;
}

inline void Tsa::SpacePoint::addToClusters(Tsa::Cluster* value){
   m_clusters.push_back(value);
}

inline void Tsa::SpacePoint::removeFromClusters(const Tsa::Cluster* value){
  std::vector<Tsa::Cluster*>::iterator iter =
    std::remove(m_clusters.begin(), m_clusters.end(), value);
  m_clusters.erase(iter, m_clusters.end());
}

inline unsigned int Tsa::SpacePoint::station() const{
  return m_station;
}

inline Tsa::SpacePoint::pointType Tsa::SpacePoint::type() const{
  return m_type;
}

inline unsigned int Tsa::SpacePoint::size() const{
  return m_clusters.size();
}

inline unsigned int Tsa::SpacePoint::pass() const{
  return m_pass;
}

inline void Tsa::SpacePoint::updatePass(const unsigned int pass) {
  m_pass = pass;
}

inline bool Tsa::SpacePoint::isIT() const{
   
  return ((m_type == OB)||(m_type == OT) ? false : true);
}

//Defintion of keyed container for TsaSpacePoint
namespace Tsa{
  typedef KeyedContainer<Tsa::SpacePoint, Containers::HashMap> SpacePoints;
  typedef std::vector<Tsa::SpacePoint*> SpacePointVector; 
};
#endif
