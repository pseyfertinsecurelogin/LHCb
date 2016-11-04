// Include files 

// local
#include "MuonDAQ/MuonHLTDigitFormat.h"
#include "Event/MuonBankVersion.h"
//-----------------------------------------------------------------------------
// Implementation file for class : MuonHLTDigitFormat
//
// 2004-01-23 : Alessia Satta
//-----------------------------------------------------------------------------


//=============================================================================
void MuonHLTDigitFormat::setAddress(unsigned int num)
{
 unsigned int tmp1=0 , tmp2=0 ;
 if(m_type==MuonBankVersion::DC06)
 {
   
   tmp1 = ( num << MuonHLTBaseDC06::ShiftAddress ) & MuonHLTBaseDC06::MaskAddress ;
   tmp2 = m_data & ~MuonHLTBaseDC06::MaskAddress ;
 }
 else if(m_type==MuonBankVersion::v1)
 {
   tmp1 = ( num << MuonHLTBaseV1::ShiftAddress ) & MuonHLTBaseV1	::MaskAddress ;
   tmp2 = m_data & ~MuonHLTBaseV1::MaskAddress ;
 }
 m_data = ( tmp1 | tmp2 ) ;
}

void MuonHLTDigitFormat::setTime(unsigned int num)
{
  unsigned int tmp1=0 , tmp2=0 ; 
  if(m_type==MuonBankVersion::DC06){
    tmp1 = ( num << MuonHLTBaseDC06::ShiftTime ) & MuonHLTBaseDC06::MaskTime ;
    tmp2 = m_data & ~MuonHLTBaseDC06::MaskTime ; 
  }else if(m_type==MuonBankVersion::v1)
  {
    tmp1 = ( num << MuonHLTBaseV1::ShiftTime ) & MuonHLTBaseV1::MaskTime ;
    tmp2 = m_data & ~MuonHLTBaseV1::MaskTime ; 
  }
  
  
  m_data = ( tmp1 | tmp2 ) ;
}

unsigned int MuonHLTDigitFormat::getTime() const
{
  if(m_type==MuonBankVersion::DC06){
    return  ( m_data & MuonHLTBaseDC06::MaskTime ) >> 
      MuonHLTBaseDC06::ShiftTime ;
  } 
  else if(m_type==MuonBankVersion::v1)
  { 
    return  ( m_data & MuonHLTBaseV1::MaskTime ) >> 
      MuonHLTBaseV1::ShiftTime ;
  }
  else
    return 0;
}

unsigned int MuonHLTDigitFormat::getAddress() const
{
  if(m_type==MuonBankVersion::DC06){
    return  ( m_data & MuonHLTBaseDC06::MaskAddress ) >> 
      MuonHLTBaseDC06::ShiftAddress ;  
  }
  else if(m_type==MuonBankVersion::v1)
  {
    return  ( m_data & MuonHLTBaseV1::MaskAddress ) >> 
      MuonHLTBaseV1::ShiftAddress ;
  }
  else
    return 0;

}

