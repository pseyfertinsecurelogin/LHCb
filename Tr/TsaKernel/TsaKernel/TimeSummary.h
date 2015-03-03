#ifndef _TimeSummary_H
#define _TimeSummary_H 1

/** @class TimeSummary TimeSummary.h
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
#include "GaudiKernel/DataObject.h"


// Namespace for locations in TDS
namespace TimeSummaryLocation {
  static const std::string& Default = "/Event/Rec/Tsa/TimeSummary";
}

// Class ID definition
static const CLID CLID_TimeSummary = 19111;

class TimeSummary: public DataObject{

public:

  /// constructer
  TimeSummary():m_total(0){}

  /// Destructor
  virtual ~TimeSummary(){}

  virtual const CLID& clID() const;
  static const CLID& classID();

  void update(const double value);

  double time() const;
  
private:

  double m_total;

};

inline const CLID& TimeSummary::clID() const{
  return TimeSummary::classID();
}

inline const CLID& TimeSummary::classID(){
  return CLID_TimeSummary;
}

inline void TimeSummary::update(const double value){
  m_total += value;
}

inline double TimeSummary::time() const{
  return m_total;
}

#endif
