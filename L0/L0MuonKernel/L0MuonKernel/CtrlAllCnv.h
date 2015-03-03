#ifndef L0MUONKERNEL_CTRLALLCNV_H
#define L0MUONKERNEL_CTRLALLCNV_H 

/** @class CtrlAllCnv CtrlAllCnv.h  L0MuonKernel/CtrlAllCnv.h
 
  Class for constructing the raw data generated by the TELL1 linked to the processing boards in transparent mode. 

  @author
  @date

*/
#include "L0MuonKernel/CandRegisterHandler.h"
#include <string>
#include <map>
#include <vector>

namespace L0Muon {
  
  class CtrlAllCnv  {

  public:

    /// Default Constructor
    CtrlAllCnv();

    /// Default Constructor
    CtrlAllCnv(int side);

    /// Destructor
    ~CtrlAllCnv();
    
    void release();

    std::vector<PMuonCandidate> muonCandidates();

    void decodeBank(std::vector<unsigned int> raw, int version);
    std::vector<unsigned int> rawBank(int version);
    
    void dump(std::string tab=""){
      for (int i=0; i<2; ++i) {
        for (int ib=0; ib<12; ++ib) 
        {
          m_candRegHandler[i][ib].dump(-1,tab);
        }      
      }
    }

  private:
    int m_side;
    // Input candidate registers 
    CandRegisterHandler m_candRegHandler[2][12];

  };
}; // namespace L0Muon
 
#endif    // L0MUONKERNEL_CTRLALLCNV_H
