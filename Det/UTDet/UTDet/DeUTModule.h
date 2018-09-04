/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef _DEUTMODULE_H_
#define _DEUTMODULE_H_

#include <vector>
#include <string>

#include "Kernel/UTChannelID.h"
#include "UTDet/DeUTBaseElement.h"

class DeUTSensor;
class DeUTLayer;

/** @class DeUTModule DeUTModule.h UTDet/DeUTModule.h
 *
 *  Class representing a UT module (13 or 7 sensors)
 *
 *  @author Jianchun Wang, Matt Needham
 *  @date   2012-07-10
 *
 */

static const CLID CLID_DeUTModule = 9310;

class DeUTModule : public DeUTBaseElement  {

public:

  /** parent type */
  typedef UTDetTraits<DeUTModule>::parent parent_type;

  /** child type */
  typedef UTDetTraits<DeUTModule>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeUTModule ( const std::string& name = "" ) ;

  /**
  * Retrieves reference to class identifier
  * @return the class identifier for this class
  */
  static const CLID& classID(){return CLID_DeUTModule;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /** region where module is located
  * @return m_region
  */
  unsigned int detRegion() const;

  /// Workaround to prevent hidden base class function
  inline const std::type_info& type(const std::string &name) const override
                      {return ParamValidDataObject::type(name);}
  /** indicate the module type (A/B/C/D)
  * @return m_type
  */
  std::string type() const;

  /** indicate the module is rotated around Z or not
  * @return m_moduleRotZ
  */
  std::string moduleRotZ() const;

  /** first readout sector on module
  * @return m_firstSector
  */
  unsigned int firstSector() const;

  /** number of readout sectors expected
  * @return m_numSectors
  */
  unsigned int numSectorsExpected() const;

  /** last readout sector on module
  * @return m_firstSector
  */
  unsigned int lastSector() const;

  /** test whether contains channel
  * @param  aChannel test channel
  * @return bool
  */
  bool contains(const LHCb::UTChannelID aChannel) const override;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to stream */
  MsgStream& printOut( MsgStream& os) const override;

  /**  locate sector based on a channel id
  @return  sector */
  DeUTSector* findSector(const LHCb::UTChannelID aChannel);

  /** locate sector based on a point
  @return sector */
  DeUTSector* findSector(const Gaudi::XYZPoint& point) ;

  /** children */
  const DeUTModule::Children& sectors() const;

  /** column number */
  unsigned int column() const;

  /** production id */
  unsigned int prodID() const;

  /**
  * fraction active channels
  * @return bool fraction active
  */
  double fractionActive() const;

  /** version */
  std::string versionString() const;

protected:

  StatusCode updateProdIDCondition();

private:

  unsigned int m_detRegion = 0;
  unsigned int m_firstSector = 0;
  unsigned int m_column = 0;
  std::string  m_type;
  std::string  m_moduleRotZ;
  unsigned int m_numSectors = 0;
  parent_type* m_parent = nullptr;
  Children     m_sectors;
  unsigned int m_prodID = 0;
  std::string  m_versionString = "DC11";
  std::string  m_prodIDString = "ProdID";
};

#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTSector.h"

inline const DeUTModule::Children& DeUTModule::sectors() const{
  return m_sectors;
}

inline unsigned int DeUTModule::detRegion() const {
  return m_detRegion;
}

inline std::string DeUTModule::type() const {
  return m_type;
}

inline std::string DeUTModule::moduleRotZ() const {
  return m_moduleRotZ;
}

inline unsigned int DeUTModule::lastSector() const {
  return firstSector()+m_sectors.size()-1u;
}

inline unsigned int DeUTModule::firstSector() const {
  return m_firstSector;
}

inline unsigned int DeUTModule::numSectorsExpected() const {
  return m_numSectors;
}

inline unsigned int DeUTModule::column() const{
  return m_column;
}

inline bool DeUTModule::contains(const LHCb::UTChannelID aChannel) const{
  return(( aChannel.detRegion() == m_detRegion &&
           aChannel.sector() >= m_firstSector &&
           aChannel.sector() < m_firstSector + m_sectors.size()) &&
         m_parent->contains(aChannel)) ;
}

inline unsigned int DeUTModule::prodID() const{
  return m_prodID;
}

inline std::string DeUTModule::versionString() const
{
  return m_versionString;
}


/** ouput operator for class DeUTModule
 *  @see DeModule
 *  @see MsgStream
 *  @param os      reference to STL output stream
 *  @param aModule reference to DeUTModule object
 */
inline std::ostream& operator<<( std::ostream& os , const DeUTModule* aModule )
{ return aModule->printOut( os ); }


/** ouput operator for class DeUTModule
 *  @see DeUTModule
 *  @see MsgStream
 *  @param os      reference to MsgStream output stream
 *  @param aModule reference to DeUTModule object
 */
inline MsgStream& operator<<( MsgStream& os , const DeUTModule* aModule )
{ return aModule->printOut( os ); }


#endif // _DEUTMODULE_H
