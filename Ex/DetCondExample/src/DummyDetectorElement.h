// $Id: DummyDetectorElement.h,v 1.2 2006-02-01 19:50:30 marcocle Exp $
#ifndef CMT_DUMMYDETECTORELEMENT_H
#define CMT_DUMMYDETECTORELEMENT_H 1

#include <string>
#include <vector>

#include "CLID_DummyDetectorElement.h"
// Include files
#include "DetDesc/DetectorElement.h"
#include "GaudiKernel/ClassID.h"
#include "GaudiKernel/StatusCode.h"

template <class T> class DataObjectFactory;

/** @class DummyDetectorElement DummyDetectorElement.h src/DummyDetectorElement.h
 *
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-04-11
 */
class DummyDetectorElement: public DetectorElement {

  friend class DataObjectFactory<DummyDetectorElement>;

public:
  /// Standard constructor
  DummyDetectorElement(const std::string& name = "NotYetDefined");

  virtual ~DummyDetectorElement( ); ///< Destructor

  /**
   *
   */
  StatusCode initialize() override;

  /// Obtain class ID
  const  CLID& clID()   const   override { return classID(); }
  static const   CLID& classID()       { return CLID_DummyDetectorElement; }

protected:

private:

  double              m_slowTemp;
  std::vector<double> m_cratesTemp;
  int                 m_maxChannels;
  std::vector<int>    m_activeChannels;

  StatusCode i_updateTemperatures();
  StatusCode i_updateChannels();

};
#endif // CMT_DUMMYDETECTORELEMENT_H
