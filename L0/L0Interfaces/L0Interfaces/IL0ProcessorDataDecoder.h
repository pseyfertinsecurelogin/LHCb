// ============================================================================
#ifndef L0DU_IL0PROCESSORDATADECODER_H
#define L0DU_IL0PROCESSORDATADECODER_H

// STD & STL 
#include <string>
#include <vector>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// from Event
#include "Event/L0ProcessorData.h"
#include "Event/L0DUBase.h"

/** @class IL0ProcessorDataDecoder IL0ProcessorDataDecoder.h
 * 
 * Generic interface for tools decoding the container of L0Processor data
 *
 * ProcessorDataDecoder : L0DU/L0ProcessorDataDecoder
 *
 * [replace the obsolete IL0Candidate interface] 
 *
 */

struct IL0ProcessorDataDecoder : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IL0ProcessorDataDecoder, 3, 0 );
    
  /** The main methods
   *
   */
  virtual bool   setL0ProcessorData(std::vector<std::string> dataLocs )=0;
  virtual bool   setL0ProcessorData(std::vector<LHCb::L0ProcessorDatas*> datass ) = 0;
  virtual bool   setL0ProcessorData(std::string dataLoc )=0;
  virtual bool   setL0ProcessorData(LHCb::L0ProcessorDatas* datas ) = 0;
  virtual double value(  const unsigned int  data[L0DUBase::Index::Size],int bx=0) = 0;
  virtual unsigned long digit(  const unsigned int  data[L0DUBase::Index::Size],int bx=0) = 0;
  virtual std::vector<int> bxList(  const unsigned int  data[L0DUBase::Index::Size]) = 0;
  virtual bool   isValid()=0;
  
};
#endif // L0DU_IL0PROCESSORDATADECODER_H 
