#ifndef L0DU_L0PROCESSORDATADECODER_H
#define L0DU_L0PROCESSORDATADECODER_H

// from GaudiKernel
#include "GaudiAlg/GaudiTool.h"
// Interfaces
#include  "L0Interfaces/IL0ProcessorDataDecoder.h"
#include  "L0Interfaces/IL0CondDBProvider.h"

class L0ProcessorDataDecoder : public GaudiTool, virtual public IL0ProcessorDataDecoder{
 public:

  L0ProcessorDataDecoder ( const std::string& type   ,
                   const std::string& name   ,
                   const IInterface*  parent ) ;

  StatusCode initialize ()  override;
  StatusCode finalize ()  override;
  bool   setL0ProcessorData(std::vector<std::string> dataLocs ) override;
  bool   setL0ProcessorData(std::vector<LHCb::L0ProcessorDatas*> datass ) override;
  bool   setL0ProcessorData(std::string dataLoc ) override;
  bool   setL0ProcessorData(LHCb::L0ProcessorDatas* datas ) override;
  unsigned long digit( const std::array<unsigned int,L0DUBase::Index::Size>& data,int bx=0) override;
  double value( const std::array<unsigned int,L0DUBase::Index::Size>& base,int bx=0) override;
  std::vector<int> bxList( const std::array<unsigned int,L0DUBase::Index::Size>& base) override;
  bool   isValid() override {return m_ok;}

private:
  LHCb::L0ProcessorDatas m_dataContainer;
  IL0CondDBProvider* m_condDB = nullptr;
  bool   m_ok = false;
  bool   m_hasHC = false;
};

#endif // L0DU_L0PROCESSORDATADECODER_H
