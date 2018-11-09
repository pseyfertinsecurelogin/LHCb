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
#ifndef      L0MUON_L0DUBASE_H
#define      L0MUON_L0DUBASE_H 1
#include <string>
#include <map>
#include <array>

namespace L0DUBase{


  /*
    L0DU firmware limitations
  */
  namespace NumberOf{
    constexpr unsigned int Compounds    = 8;
    constexpr unsigned int Data        = 28;
    constexpr unsigned int Channels    = 32;
    constexpr unsigned int Conditions  =128;
    constexpr unsigned int ConditionsInBank  =32;
  }

  namespace CompoundData{
    enum Type {None=0,CaloEt,CaloAdd,PuCont,PuPos,MuonPt,MuonAdd,MuonSgn};
    inline const std::array<std::string,NumberOf::Compounds> Name = {"None",
                                                          "CompoundCaloEt",
                                                          "CompoundCaloAdd",
                                                          "CompoundPuCont",
                                                          "CompoundPuPos",
                                                          "CompoundMuonPt",
                                                          "CompoundMuonAdd",
                                                          "CompoundMuonSgn"};


    inline const std::array<unsigned int, NumberOf::Compounds> MaxNumber = { 0, 4, 10, 4, 2, 4, 6, 6 };
    inline const std::array<unsigned int, NumberOf::Compounds> ConditionOrder  = { 0, 19,20,21,22,23,24,25 };
    inline const std::array<unsigned int, NumberOf::Compounds> OperatorType= { 0, 1,2,1,1,1,2,1 };
  }

  namespace PredefinedData{
    enum Type {ElectronEt=0,PhotonEt, GlobalPi0Et,LocalPi0Et,HadronEt,
               SumEt,SpdMult,
               PuPeak1,PuPeak2,
               PuPeak1Pos,PuPeak2Pos,
               PuHits,
               Muon1Pt,Muon2Pt,Muon3Pt,DiMuonPt,DiMuonProdPt,
               ElectronAdd,PhotonAdd,GlobalPi0Add,LocalPi0Add,HadronAdd,
               Muon1Add,Muon2Add,Muon3Add,
               Muon1Sgn,Muon2Sgn,Muon3Sgn};

    inline const std::array<std::string,NumberOf::Data> Name = {
      "Electron(Et)", "Photon(Et)", "GlobalPi0(Et)", "LocalPi0(Et)","Hadron(Et)","Sum(Et)","Spd(Mult)",
      "PUPeak1(Cont)","PUPeak2(Cont)","PUPeak1(Pos)","PUPeak2(Pos)","PUHits(Mult)",
      "Muon1(Pt)","Muon2(Pt)","Muon3(Pt)","DiMuon(Pt)","DiMuonProd(Pt1Pt2)",
      "Electron(Add)", "Photon(Add)", "GlobalPi0(Add)", "LocalPi0(Add)","Hadron(Add)",
      "Muon1(Add)","Muon2(Add)","Muon3(Add)",
      "Muon1(Sgn)","Muon2(Sgn)","Muon3(Sgn)",
    };

    // Herschel aliases
    inline const std::map<std::string,std::string> Alias={
      {"HRC(F)","LocalPi0(Et)"},
      {"HRC(B)","GlobalPi0(Et)"},
      {"HRCF(Add)","LocalPi0(Add)"},
      {"HRCB(Add)","GlobalPi0(Add)"}
    };

    inline const std::array<unsigned int, NumberOf::Data> CompoundType = {
      CompoundData::CaloEt,CompoundData::CaloEt,CompoundData::CaloEt,CompoundData::CaloEt,CompoundData::CaloEt,
      CompoundData::None,CompoundData::None,
      CompoundData::PuCont,CompoundData::PuCont,
      CompoundData::PuPos,CompoundData::PuPos,
      CompoundData::None,
      CompoundData::MuonPt,CompoundData::MuonPt,CompoundData::MuonPt,CompoundData::None,CompoundData::None,
      CompoundData::CaloAdd,CompoundData::CaloAdd,CompoundData::CaloAdd,CompoundData::CaloAdd,CompoundData::CaloAdd,
      CompoundData::MuonAdd,CompoundData::MuonAdd,CompoundData::MuonAdd,
      CompoundData::MuonSgn,CompoundData::MuonSgn,CompoundData::MuonSgn
    };

    // - OD - 2016 :  Pi0G/Pi0L/SumEt  :  5/5/4  => 3/3/8
    inline const std::array<unsigned int,NumberOf::Data>  MaxNumber={8,6,3,3,8,    // ElectronEt, PhotonEt, GlobalPi0Et, LocalPi0Et, HadronEt
                                                         8,8,          // SumEt , SpdMult
                                                         4,4,          // PuPeak1, PUPeak2
                                                         2,2,          // PuPeak1Pos, PuPeak2Pos
                                                         6,            // PuHits
                                                         10,5,3,4,4,   // Muon1, Muon2, Muon3, DiMuon, MuonProd
                                                         0,0,0,0,0,    // Calo Addresses
                                                         0,0,0,        // Muon Addresses
                                                         0,0,0};       // Muon Sign

    inline const std::array<unsigned int, NumberOf::Data> ConditionOrder={1,2,3,4,5,
                                                              6,7,
                                                              8,9,10,
                                                              11,12,
                                                              13,14,15,16,17,
                                                              // BCID + compound Data inserted here (1+7 compound types)
                                                              26,27,28,29,30,
                                                              31,32,33,
                                                              34,35,36};
    }

  namespace RAMBCID{
    constexpr unsigned int Max = 255;
    constexpr unsigned int ConditionOrder = 18;
    constexpr unsigned int MaxNumber = 4;
    inline const std::string  Name = "RamBCID";
  }


  /*
    Empty L0Processor data
   */
  constexpr unsigned int EmptyData = 1 << 16;

  /*
    Calo candidate types as defined in EDMS-845277
  */
  namespace CaloType {
    enum Type {Electron=0, Photon, Hadron,Pi0Local,Pi0Global,SumEt,SpdMult,
               HadronSlave1Out,HadronSlave2Out,HadronSlave1In,HadronSlave2In,
               SumEtSlave1Out,SumEtSlave2Out,SumEtSlave1In,SumEtSlave2In};
  }

  /*
    L0DU Fiber numbering : FOLLOW THE HARDWARE ORDERING (needed to decode the Status bit from raw)
  */
  namespace Fiber {
    enum Type{ Pu1=0,Pu2,CaloSumEt,CaloSpdMult,CaloHadron,CaloPi0Global,CaloPi0Local,CaloPhoton,CaloElectron,Spare1,Spare2,Spare3
               , MuonCU0,MuonSU0,MuonCU1,MuonSU1,MuonCU2,MuonSU2,MuonCU3,MuonSU3,Spare4, Spare5, Spare6, Spare7
               , Empty }; // MUST BE ENDED WITH 'EMPTY' fibre
  }

  /*
     Data scale :
  */
  namespace Type {
    enum Scale{ Digit = 0 ,  MuonPt , CaloEt };
  }


  /*
    Shifts & Masks needed for the definition of the L0Processor Data Pattern
  */

  namespace Calo {
    namespace Et      { constexpr unsigned int Mask  = 0x1FE      , Shift = 1;  } // e,p,h,pi0G,pi0L
    namespace Address { constexpr unsigned int Mask  = 0x7FFE0000 , Shift = 17; }
    namespace BCID    { constexpr unsigned int Mask  = 0xFE00     , Shift = 9; }
    namespace Status  { constexpr unsigned int Mask  = 0x80000000 , Shift = 31; }
    namespace Sum     { constexpr unsigned int Mask  = 0x7FFE0000 , Shift = 17; } // SpdMult & SumEt
  }
  namespace Muon {
    // from CU
    namespace Pt2      { constexpr unsigned int Mask  = 0xFE       , Shift = 1;  }
    namespace Address2 { constexpr unsigned int Mask  = 0x7F00     , Shift = 8;  }
    namespace BCID0    { constexpr unsigned int Mask  = 0x8000     , Shift = 15;  }
    namespace Pt1      { constexpr unsigned int Mask  = 0xFE0000   , Shift = 17; }
    namespace Address1 { constexpr unsigned int Mask  = 0x7F000000 , Shift = 24; }
    namespace BCID1    { constexpr unsigned int Mask  = 0x80000000 , Shift = 31;  }
    // from SU
    namespace Pu2      { constexpr unsigned int Mask  = 0x6       , Shift = 1; } // PU address
    namespace Pb2      { constexpr unsigned int Mask  = 0x78      , Shift = 3; } // PB address
    namespace Sign2    { constexpr unsigned int Mask  = 0x80      , Shift = 7; }
    namespace BCID     { constexpr unsigned int Mask  = 0xFE00    , Shift = 9; }
    namespace Pu1      { constexpr unsigned int Mask  = 0x60000   , Shift = 17;} // PU address
    namespace Pb1      { constexpr unsigned int Mask  = 0x780000  , Shift = 19;} // PB address
    namespace Sign1    { constexpr unsigned int Mask  = 0x800000  , Shift = 23; }
    namespace Status   { constexpr unsigned int Mask  = 0xF0000000, Shift = 28; }
    constexpr unsigned int AddressSize = 7;
    constexpr unsigned int BCID0Size = 7;
  }
  // L0Pu
  namespace L0Pu    {
    //  Same Pattern for both content and address of Peak1 and  Peak2
    namespace Peak    { constexpr unsigned int Mask  = 0x1FE      , Shift = 1;  }
    namespace Address { constexpr unsigned int Mask  = 0x1FE0000  , Shift = 17; }
    namespace Status  { constexpr unsigned int Mask  = 0x80000000 , Shift = 31; }
    // specific to Pu1
    namespace BCID1   { constexpr unsigned int Mask  = 0xFE00     , Shift = 9;  }
    namespace MoreInfo{ constexpr unsigned int Mask  = 0xF000000  , Shift = 24; }
    // specific to Pu2
    namespace BCID2   { constexpr unsigned int Mask  = 0xF800     , Shift = 11;  }
    namespace HitsLSB { constexpr unsigned int Mask  = 0x600      , Shift = 9;  }
    namespace HitsMSB { constexpr unsigned int Mask  = 0x7E000000 , Shift = 25;  }
    constexpr unsigned int HitsLSBSize = 2 ;
  }

  /*
    Hard coded cross-references
  */

  namespace Index{
    enum Position {Fiber=0, Scale , Mask, Shift, Fiber2, Mask2, Shift2, Offset };
    constexpr int Size = 8;
  }

  namespace Electron{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloElectron , Type::CaloEt, Calo::Et::Mask , Calo::Et::Shift , Fiber::Empty,0,0,0,},
      Address = { Fiber::CaloElectron, Type::Digit ,Calo::Address::Mask, Calo::Address::Shift, Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloElectron, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloElectron, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Photon{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloPhoton , Type::CaloEt, Calo::Et::Mask , Calo::Et::Shift , Fiber::Empty,0,0,0},
      Address = { Fiber::CaloPhoton, Type::Digit ,Calo::Address::Mask, Calo::Address::Shift, Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloPhoton, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloPhoton, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Hadron{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloHadron , Type::CaloEt, Calo::Et::Mask , Calo::Et::Shift , Fiber::Empty,0,0,0},
      Address = { Fiber::CaloHadron, Type::Digit ,Calo::Address::Mask, Calo::Address::Shift, Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloHadron, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloHadron, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Pi0Global{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloPi0Global , Type::CaloEt, Calo::Et::Mask , Calo::Et::Shift , Fiber::Empty,0,0,0},
      Address = { Fiber::CaloPi0Global, Type::Digit ,Calo::Address::Mask, Calo::Address::Shift, Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloPi0Global, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloPi0Global, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Pi0Local{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloPi0Local , Type::CaloEt, Calo::Et::Mask , Calo::Et::Shift , Fiber::Empty,0,0,0},
      Address = { Fiber::CaloPi0Local, Type::Digit ,Calo::Address::Mask, Calo::Address::Shift, Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloPi0Local, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloPi0Local, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Sum{
    constexpr std::array<unsigned int,Index::Size>
      Et = { Fiber::CaloSumEt , Type::CaloEt, Calo::Sum::Mask , Calo::Sum::Shift , Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloSumEt, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloSumEt, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Spd{
    constexpr std::array<unsigned int,Index::Size>
      Mult = { Fiber::CaloSpdMult, Type::Digit, Calo::Sum::Mask , Calo::Sum::Shift , Fiber::Empty,0,0,0},
      BCID  = { Fiber::CaloSpdMult, Type::Digit ,Calo::BCID::Mask,Calo::BCID::Shift,Fiber::Empty,0,0,0},
      Status  = { Fiber::CaloSpdMult, Type::Digit ,Calo::Status::Mask,Calo::Status::Shift,Fiber::Empty,0,0,0};
  }

  namespace Muon1{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU0, Type::MuonPt,Muon::Pt1::Mask, Muon::Pt1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU0, Type::Digit ,Muon::Sign1::Mask, Muon::Sign1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU0, Type::Digit ,Muon::Address1::Mask , Muon::Address1::Shift ,
                  Fiber::MuonSU0, Muon::Pu1::Mask|Muon::Pb1::Mask , Muon::Pu1::Shift,
                  Muon::AddressSize},
      BCID1    = { Fiber::MuonCU0, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::Empty                , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU0, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU0, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};

  }
  namespace Muon2{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU0, Type::MuonPt,Muon::Pt2::Mask, Muon::Pt2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU0, Type::Digit ,Muon::Sign2::Mask, Muon::Sign2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU0, Type::Digit ,Muon::Address2::Mask , Muon::Address2::Shift ,
                  Fiber::MuonSU0, Muon::Pu2::Mask|Muon::Pb2::Mask ,
                  Muon::Pu2::Shift,Muon::AddressSize},
      BCID1    = { Fiber::MuonCU0, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU0              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU0, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU0, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Muon3{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU1, Type::MuonPt,Muon::Pt1::Mask, Muon::Pt1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU1, Type::Digit ,Muon::Sign1::Mask, Muon::Sign1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU1, Type::Digit ,Muon::Address1::Mask , Muon::Address1::Shift ,
                  Fiber::MuonSU1, Muon::Pu1::Mask|Muon::Pb1::Mask ,
                  Muon::Pu1::Shift,Muon::AddressSize},
      BCID1    = { Fiber::MuonCU1, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU1              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU1, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU1, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Muon4{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU1, Type::MuonPt,Muon::Pt2::Mask, Muon::Pt2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU1, Type::Digit ,Muon::Sign2::Mask, Muon::Sign2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU1, Type::Digit ,Muon::Address2::Mask , Muon::Address2::Shift ,
                  Fiber::MuonSU1, Muon::Pu2::Mask|Muon::Pb2::Mask , Muon::Pu2::Shift,
                  Muon::AddressSize},
      BCID1    = { Fiber::MuonCU1, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU1              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU1, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU1, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }

  namespace Muon5{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU2, Type::MuonPt,Muon::Pt1::Mask, Muon::Pt1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU2, Type::Digit ,Muon::Sign1::Mask, Muon::Sign1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU2, Type::Digit ,Muon::Address1::Mask , Muon::Address1::Shift ,
                  Fiber::MuonSU2, Muon::Pu1::Mask|Muon::Pb1::Mask , Muon::Pu1::Shift,
                  Muon::AddressSize},
      BCID1    = { Fiber::MuonCU2, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU2              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU2, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU2, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Muon6{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU2, Type::MuonPt,Muon::Pt2::Mask, Muon::Pt2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU2, Type::Digit ,Muon::Sign2::Mask, Muon::Sign2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU2, Type::Digit ,Muon::Address2::Mask , Muon::Address2::Shift ,
                  Fiber::MuonSU2, Muon::Pu2::Mask|Muon::Pb2::Mask , Muon::Pu2::Shift,
                  Muon::AddressSize},
      BCID1    = { Fiber::MuonCU2, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU2              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU2, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU2, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Muon7{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU3, Type::MuonPt,Muon::Pt1::Mask, Muon::Pt1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU3, Type::Digit ,Muon::Sign1::Mask, Muon::Sign1::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU3, Type::Digit ,Muon::Address1::Mask , Muon::Address1::Shift ,
                  Fiber::MuonSU3, Muon::Pu1::Mask|Muon::Pb1::Mask , Muon::Pu1::Shift,
                  Muon::AddressSize},
      BCID1    = { Fiber::MuonCU3, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                   Fiber::MuonCU3              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
      BCID2    = { Fiber::MuonSU3, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
      Status  = { Fiber::MuonSU3, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }
  namespace Muon8{
    constexpr std::array<unsigned int,Index::Size>
      Pt  = { Fiber::MuonCU3, Type::MuonPt,Muon::Pt2::Mask, Muon::Pt2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Sign= { Fiber::MuonSU3, Type::Digit ,Muon::Sign2::Mask, Muon::Sign2::Shift, Fiber::Empty, 0 , 0 , 0 },
      Address = { Fiber::MuonCU3, Type::Digit ,Muon::Address2::Mask , Muon::Address2::Shift ,
                               Fiber::MuonSU3, Muon::Pu2::Mask|Muon::Pb2::Mask , Muon::Pu2::Shift,
                               Muon::AddressSize},
      BCID1    = { Fiber::MuonCU3, Type::Digit , Muon::BCID0::Mask  , Muon::BCID0::Shift  ,
                                Fiber::MuonCU3              , Muon::BCID1::Mask  , Muon::BCID1::Shift  , Muon::BCID0Size},
       BCID2    = { Fiber::MuonSU3, Type::Digit , Muon::BCID::Mask  , Muon::BCID::Shift  ,Fiber::Empty,0,0,0},
       Status  = { Fiber::MuonSU3, Type::Digit , Muon::Status::Mask, Muon::Status::Shift,Fiber::Empty,0,0,0};
  }

  namespace PileUp{
    constexpr std::array<unsigned int,Index::Size>
      Peak1 = { Fiber::Pu1, Type::Digit , L0Pu::Peak::Mask, L0Pu::Peak::Shift, Fiber::Empty, 0 , 0 , 0  },
      Peak2 = { Fiber::Pu2, Type::Digit , L0Pu::Peak::Mask, L0Pu::Peak::Shift, Fiber::Empty, 0 , 0 , 0  },
      Peak1Pos = { Fiber::Pu1, Type::Digit , L0Pu::Address::Mask, L0Pu::Address::Shift, Fiber::Empty,
                                0 , 0 , 0 },
      Peak2Pos = { Fiber::Pu2, Type::Digit , L0Pu::Address::Mask, L0Pu::Address::Shift, Fiber::Empty,
                                  0 , 0 , 0 },
      Hits  = {Fiber::Pu2, Type::Digit, L0Pu::HitsLSB::Mask, L0Pu::HitsLSB::Shift,
                                Fiber::Pu2, L0Pu::HitsMSB::Mask, L0Pu::HitsMSB::Shift, L0Pu::HitsLSBSize  },
      BCID1   = { Fiber::Pu1, Type::Digit , L0Pu::BCID1::Mask  , L0Pu::BCID1::Shift  ,Fiber::Empty,0,0,0},
      BCID2   = { Fiber::Pu2, Type::Digit , L0Pu::BCID2::Mask  , L0Pu::BCID2::Shift  ,Fiber::Empty,0,0,0},
      Status1 = { Fiber::Pu1, Type::Digit , L0Pu::Status::Mask , L0Pu::Status::Shift  ,Fiber::Empty,0,0,0},
      Status2 = { Fiber::Pu2, Type::Digit , L0Pu::Status::Mask , L0Pu::Status::Shift  ,Fiber::Empty,0,0,0},
      MoreInfo= { Fiber::Pu1, Type::Digit , L0Pu::MoreInfo::Mask, L0Pu::MoreInfo::Shift,Fiber::Empty,0,0,0};
  }
}

#endif // L0MUON_L0DUBASE_H
