// $Id: DeITBox.h,v 1.6 2008-07-14 07:38:36 mneedham Exp $
#ifndef _DeITBox_H_
#define _DeITBox_H_

#include <string>
#include <vector>
#include "STDet/DeSTBaseElement.h"

class DeITLayer;
class DeITStation;

/** @class DeITBox DeITBox.h "STDet/DeITBox.h"
 *
 *  IT Box detector element
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on STDet</b></a>
   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *  @author Matthew Needham Matthew.Needham@cern.ch
 */

static const CLID CLID_DeITBox = 9203; // needs fixing !

class DeITBox : public DeSTBaseElement  {

public:

   /** parent type */
   typedef STDetTraits<DeITBox>::parent parent_type;

   /** child type */
   typedef STDetTraits<DeITBox>::child child_type;

   /** children */
   typedef std::vector<child_type*> Children;

  /** Constructor */
  DeITBox ( const std::string& name = "" ) ;

  /** Destructor */
  virtual ~DeITBox(); 

   /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
   static const CLID& classID(){return CLID_DeITBox;}
                        
   /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
   const CLID& clID () const;
                                                        
   /** initialization method 
   * @return Status of initialisation
   */
   virtual StatusCode initialize();


   /** check whether contains 
   *  @param aChannel channel
   *  @return bool
   */ 
   bool contains(const LHCb::STChannelID aChannel) const; 

   /** identifier */
   unsigned int id() const;

   /**  locate half module based on a channel id
   @return  module */
   DeITLayer* findLayer(const LHCb::STChannelID aChannel);     

   /** locate half module  based on a point  
   @return module */
   DeITLayer* findLayer(const Gaudi::XYZPoint& point) ;  

   /** vector of children **/
   const DeITBox::Children& layers() const;

   /** print to stream */
   std::ostream& printOut( std::ostream& os ) const;
     
   /** print to msgstream */
   MsgStream& printOut( MsgStream& os) const;

   /** Nickname for the box **/   
   const std::string& nickname() const;

   /** 
   * fraction active channels
   * @return bool fraction active
   */
   double fractionActive() const;

private:

   Children m_layers;
   parent_type* m_parent;
   unsigned int m_id;
   std::string m_nickname;
};


#include "STDet/DeITBox.h"
#include "STDet/DeITStation.h"

inline unsigned int DeITBox::id() const{
  return m_id;
}

inline const DeITBox::Children& DeITBox::layers() const{
  return m_layers;
}

inline bool DeITBox::contains(const LHCb::STChannelID aChannel) const{
  return ((aChannel.detRegion() == elementID().detRegion()) 
         && m_parent->contains(aChannel)) ;
}


/** ouput operator for class DeITBox
 *  @see DeITBox
 *  @see MsgStream
 *  @param os   reference to STL output stream
 *  @param aBox reference to DeITBox object
 */
inline std::ostream& operator<<( std::ostream& os , const DeITBox* aBox )
{ return aBox->printOut( os ); }


/** ouput operator for class DeITBox
 *  @see DeITBox
 *  @see MsgStream
 *  @param os   reference to MsgStream output stream
 *  @param aBox reference to DeITBox object
 */
inline MsgStream& operator<<( MsgStream& os , const DeITBox* aBox )
{ return aBox->printOut( os ); }

inline const std::string& DeITBox::nickname() const{
  return m_nickname;
} 

#endif // _DeITBox_H







