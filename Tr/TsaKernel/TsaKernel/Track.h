#ifndef TsaEvent_Track_H
#define TsaEvent_Track_H 1

// Include files
#include <algorithm>
#include <vector>

#include "Kernel/TrackTypes.h"
#include "GaudiKernel/KeyedObject.h"

#include "GaudiKernel/ObjectList.h"
#include "GaudiKernel/ObjectVector.h"

// Forward declarations

namespace Tsa{
  class Cluster;

  // Class ID definition
  static const CLID& CLID_TsaTrack = 19010;

  // Namespace for locations in TDS
  namespace TrackLocation {
    static const std::string& Default = "/Event/Rec/Tsa/Tracks";
  }

 /** @class TsaTrack TsaTrack.h 
  *
  *  spacepoint class for Tsa
  *
  *  @author Matthew Needham
  *  created Wed Jun 25 14:16:12 2003
  *
  */

  class Track: public KeyedObject<int>{

    public: 


    ///  constructer
    Track(const double x,
           const double y,
           const double z,
           const double tx,
           const double ty, 
           const double qDivP,
           const std::vector<Cluster*>& clusters);
 
    /// Destructor 
    virtual ~Track() {}

    /// clone
    Track* clone() const;

    /// Retrieve pointer to class definition structure
    virtual const CLID& clID() const; 
    static const CLID& classID(); 

    /// short cut to x
    double x() const;

    /// short cut to y
    double y() const;

    /// short cut to z
    double z() const;

    // set z
    void setZ(const double z); 
  
    /// Retrieve measured x slope val
    double tX() const;

    /// Retrieve measured x slope val
    double tY() const;

    /// Retrieve measured Q/p
    double qDivP() const;
  
    /// get the error^2 on the x value
    double eX2() const;

    /// get the error^2 on the y value
    double eY2() const;

    /// get the error^2 on the Tx value
    double eTx2() const;

    /// get the error^2 on the Ty value
    double eTy2() const;

    /// get the error^2 on the Q/P value
    double eQdivP2() const;

    /// Retrieve state vector
    const Gaudi::TrackVector& stateVector() const; 

    /// Retrieve mewsured point (mm) (non-const)
    Gaudi::TrackVector& stateVector();

    /// Update state vector
    void setStateVector(const Gaudi::TrackVector& value);

    /// Retrieve state vector
    const Gaudi::TrackVector& covVector() const; 

    /// Retrieve mewsured point (mm) (non-const)
    Gaudi::TrackVector& covVector();

    // retrieve chi2
    double chi2() const;

    // update chi2
    void setChi2(double value);   

    /// number of clusters on track
    unsigned int nMeas() const;

    /// Retrieve smartref of clusters (const)
    const std::vector<Tsa::Cluster*>& clusters() const;

    /// Retrieve smartref of clusters (non-const)
    // std::vector<TsaCluster*>& clusters();

    /// Update smartref of clusters
    void setClusters(const std::vector<Tsa::Cluster*>& value);

    /// Add smartref of clusters
    void addToClusters(Tsa::Cluster* value); 

    /// Remove smartref of clusters
    void removeFromClusters(Tsa::Cluster* value); 

    /// Clear smartref of clusters
    void clearClusters();

  private: 

    Track(const Tsa::Track& track);
  
    Gaudi::TrackVector  m_stateVector; ///< state vector
    Gaudi::TrackVector  m_covVector; // errors...

   double m_z;
   double m_chi2; ///< chi2
   std::vector<Tsa::Cluster*> m_clusters; ///< smartref of clusters

  };
};

// -----------------------------------------------------------------------------
//   end of class
// -----------------------------------------------------------------------------


// Including forward declarations
#include "TsaKernel/Cluster.h"

inline const CLID& Tsa::Track::clID() const 
{
  return Tsa::Track::classID();
}

inline const CLID& Tsa::Track::classID()
{
  return CLID_TsaTrack;
}

inline double Tsa::Track::x() const
{
 return m_stateVector(0);    
}

inline double Tsa::Track::y() const
{  
 return m_stateVector(1);    
}

inline double Tsa::Track::z() const
{
 return m_z;    
}

inline void Tsa::Track::setZ(const double z){
  m_z = z;
}

inline double Tsa::Track::tX() const 
{
  return m_stateVector(2);
}

inline double Tsa::Track::tY() const 
{
  return m_stateVector(3);
}

inline double Tsa::Track::qDivP() const 
{
  return m_stateVector(4);
}

inline double Tsa::Track::eX2() const
{
 return m_covVector(0);    
}

inline double Tsa::Track::eY2() const
{  
 return m_covVector(1);    
}

inline double Tsa::Track::eTx2() const 
{
  return m_covVector(2);
}

inline double Tsa::Track::eTy2() const 
{
  return m_covVector(3);
}

inline double Tsa::Track::eQdivP2() const 
{
  return m_covVector(4);
}

inline const Gaudi::TrackVector& Tsa::Track::stateVector() const
{
  return m_stateVector;
}

inline Gaudi::TrackVector& Tsa::Track::stateVector()
{
  return m_stateVector;
}

inline void Tsa::Track::setStateVector(const Gaudi::TrackVector& value)
{
  m_stateVector = value;
}

inline const Gaudi::TrackVector& Tsa::Track::covVector() const
{
  return m_covVector;
}

inline Gaudi::TrackVector& Tsa::Track::covVector()
{
  return m_covVector;
}
inline double Tsa::Track::chi2() const 
{
  return m_chi2;
}

inline void Tsa::Track::setChi2(double value)
{
  m_chi2 = value; 
}

inline unsigned int Tsa::Track::nMeas() const{
  return m_clusters.size();
}

inline const std::vector<Tsa::Cluster*>& Tsa::Track::clusters() const
{
   return m_clusters;
}

//inline std::vector<Tsa::Cluster*>& Tsa::Track::clusters()
//{
//   return m_clusters;
//}

inline void Tsa::Track::setClusters(const std::vector<Tsa::Cluster*>& value)
{
   m_clusters = value;
}

inline void Tsa::Track::addToClusters(Tsa::Cluster* value)
{
   m_clusters.push_back(value);
}

inline void Tsa::Track::removeFromClusters(Tsa::Cluster* value)
{
  std::vector<Tsa::Cluster*>::iterator iter =
    std::remove(m_clusters.begin(), m_clusters.end(), value);
  m_clusters.erase(iter, m_clusters.end());
}

//Defintion of keyed container for Tsa::Track
typedef KeyedContainer<Tsa::Track, Containers::HashMap> TsaTracks;

#endif   ///TsaEvent_TsaTrack_H

