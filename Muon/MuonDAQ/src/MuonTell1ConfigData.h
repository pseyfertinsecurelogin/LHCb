/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef MUONTELL1CONFIGDATA_H
#define MUONTELL1CONFIGDATA_H 1

// Include files
#include <iomanip>
#include <array>
#include <algorithm>

/** @class MuonTell1ConfigData MuonTell1ConfigData.h
 *
 *
 *  @author
 *  @date   2008-03-01
 */
class MuonTell1ConfigData final {
public:
  /// Standard constructor
  MuonTell1ConfigData( ) {
    pad_offset.fill(0);
    hit_offset.fill(0);
    padTU.fill(0);
  };

  void setPadOffset(unsigned int* off){ std::copy_n(off,24,pad_offset.data()); }
  void setHitOffset(unsigned int* off){ std::copy_n(off,24,hit_offset.data()); }
  void setPadTU(unsigned int * off){ std::copy_n(off,24,padTU.data()); }

  void setPadOffset(unsigned int ch,unsigned int pad_off){ pad_offset[ch]=pad_off;}
  void setHitOffset(unsigned int ch,unsigned int Hit_off){ hit_offset[ch]=Hit_off;}

  void setPadTU(unsigned int ch,unsigned int pad_TU){ padTU[ch]=pad_TU;}

  unsigned int getPadOffset(int ch) const { return pad_offset[ch]; }
  unsigned int getHitOffset(int ch) const { return hit_offset[ch]; }
  unsigned int getPadTU(int ch) const { return padTU[ch]; }

  std::ostream& fillStream(std::ostream& s) const;

private:

  std::array<unsigned int,24> pad_offset;
  std::array<unsigned int,24> padTU;
  std::array<unsigned int,24> hit_offset;

};

std::ostream & MuonTell1ConfigData::fillStream(std::ostream &s)const
{
  static const char* sep[] = { ",",",",",",",",",","" };
  static const char* l[]   = { "]    --PP0, optical link 0-5\n",
                               "]    --PP1, optical link 0-5\n",
                               "]    --PP2, optical link 0-5\n",
                               "]    --PP3, optical link 0-5\n" };

  s<<"#  The TU type of each optical link. "
     "(0->Type1a, 1->Type1b,2->Type2,3->Type3,4->Type4a,"
     "5->Type4b,6->Type5,7->Type6)\n";

  s<<" _L0050:___    [";
  for(int i=0;i<6;i++) s<<std::setw(4)<<padTU[i]<<sep[i%6];
  s<< l[0];

  s<<" _L0051:___    [";
  for(int i=6;i<12;i++) s<<std::setw(4)<<padTU[i]<<sep[i%6];
  s<< l[1];

  s<<" _L0052:___    [";
  for(int i=12;i<18;i++) s<<std::setw(4)<<padTU[i]<<sep[i%6];
  s<< l[2];

  s<<" _L0053:___    [";
  for(int i=18;i<24;i++) s<<std::setw(4)<<padTU[i]<<sep[i%6];
  s<< l[3];

  s<<"#  The PAD process offset address of each optical link\n";

  s<<" _L0054:___    [";
  for(int i=0;i<6;i++) s<<std::setw(8)<<pad_offset[i]<<sep[i%6];
  s<< l[0];

  s<<" _L0055:___    [";
  for(int i=6;i<12;i++) s<<std::setw(8)<<pad_offset[i]<<sep[i%6];
  s<< l[1];

  s<<" _L0056:___    [";
  for(int i=12;i<18;i++) s<<std::setw(8)<<pad_offset[i]<<sep[i%6];
  s<< l[2];

  s<<" _L0057:___    [";
  for(int i=18;i<24;i++) s<<std::setw(8)<<pad_offset[i]<<sep[i%6];
  s<< l[3];

  s<<" #  The HIT process offset address of each optical link\n";

  s<<" _L0058:___    [";
  for(int i=0;i<6;i++) s<<std::setw(8)<<hit_offset[i]<<sep[i%6];
  s<< l[0];

  s<<" _L0059:___    [";
  for(int i=6;i<12;i++) s<<std::setw(8)<<hit_offset[i]<<sep[i%6];
  s<< l[1];

  s<<" _L0060:___    [";
  for(int i=12;i<18;i++) s<<std::setw(8)<<hit_offset[i]<<sep[i%6];
  s<< l[2];

  s<<" _L0061:___    [";
  for(int i=18;i<24;i++) s<<std::setw(8)<<hit_offset[i]<<sep[i%6];
  s<< l[3];

  return s ;
}

inline std::ostream& operator<< (std::ostream& str, const MuonTell1ConfigData& obj)
{ return obj.fillStream(str);   }

#endif // MUONTELL1CONFIGDATA_H
