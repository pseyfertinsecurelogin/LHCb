// local
#include "HltDAQ/ReportConvertTool.h"
#include "pun.h"

using LHCb::HltObjectSummary;
using LHCb::HltSelRepRBStdInfo;

namespace {

  template <typename C>
  unsigned int count_info(const C& c) {
    return std::count_if( begin(c), end(c),
                          [](typename C::const_reference elem) {
                          return elem.first.find("#")!=std::string::npos;
  } ); }

  static const std::array<LHCb::RecSummary::DataTypes,23> s_rsum_map = {
       LHCb::RecSummary::DataTypes::nLongTracks,
       LHCb::RecSummary::DataTypes::nDownstreamTracks,
       LHCb::RecSummary::DataTypes::nUpstreamTracks,
       LHCb::RecSummary::DataTypes::nVeloTracks,
       LHCb::RecSummary::DataTypes::nTTracks,
       LHCb::RecSummary::DataTypes::nBackTracks,
       LHCb::RecSummary::DataTypes::nTracks,
       LHCb::RecSummary::DataTypes::nRich1Hits,
       LHCb::RecSummary::DataTypes::nRich2Hits,
       LHCb::RecSummary::DataTypes::nVeloClusters,
       LHCb::RecSummary::DataTypes::nITClusters,
       LHCb::RecSummary::DataTypes::nTTClusters,
       LHCb::RecSummary::DataTypes::nUTClusters,
       LHCb::RecSummary::DataTypes::nOTClusters,
       LHCb::RecSummary::DataTypes::nFTClusters,
       LHCb::RecSummary::DataTypes::nSPDhits,
       LHCb::RecSummary::DataTypes::nMuonCoordsS0,
       LHCb::RecSummary::DataTypes::nMuonCoordsS1,
       LHCb::RecSummary::DataTypes::nMuonCoordsS2,
       LHCb::RecSummary::DataTypes::nMuonCoordsS3,
       LHCb::RecSummary::DataTypes::nMuonCoordsS4,
       LHCb::RecSummary::DataTypes::nMuonTracks,
       LHCb::RecSummary::DataTypes::nPVs
  };
  static const std::array<LHCb::ProtoParticle::additionalInfo,72> s_proto_map = {
      LHCb::ProtoParticle::additionalInfo::IsPhoton,//381
      LHCb::ProtoParticle::additionalInfo::IsNotE,//382
      LHCb::ProtoParticle::additionalInfo::IsNotH,//383
      LHCb::ProtoParticle::additionalInfo::EcalPIDe,//360
      LHCb::ProtoParticle::additionalInfo::PrsPIDe,//361
      LHCb::ProtoParticle::additionalInfo::BremPIDe,//362
      LHCb::ProtoParticle::additionalInfo::HcalPIDe,//363
      LHCb::ProtoParticle::additionalInfo::HcalPIDmu,//364
      LHCb::ProtoParticle::additionalInfo::EcalPIDmu,//365
      LHCb::ProtoParticle::additionalInfo::CaloTrMatch,//310
      LHCb::ProtoParticle::additionalInfo::CaloElectronMatch,//311
      LHCb::ProtoParticle::additionalInfo::CaloBremMatch,//312
      LHCb::ProtoParticle::additionalInfo::CaloNeutralSpd,//323
      LHCb::ProtoParticle::additionalInfo::CaloNeutralPrs,//324
      LHCb::ProtoParticle::additionalInfo::CaloNeutralEcal,//325
      LHCb::ProtoParticle::additionalInfo::CaloNeutralHcal2Ecal,//326
      LHCb::ProtoParticle::additionalInfo::CaloNeutralE49,//327
      LHCb::ProtoParticle::additionalInfo::CaloNeutralID,//328
      LHCb::ProtoParticle::additionalInfo::CaloDepositID,//320
      LHCb::ProtoParticle::additionalInfo::ShowerShape,//321
      LHCb::ProtoParticle::additionalInfo::ClusterMass,//322
      LHCb::ProtoParticle::additionalInfo::CaloSpdE,//330
      LHCb::ProtoParticle::additionalInfo::CaloPrsE,//331
      LHCb::ProtoParticle::additionalInfo::CaloEcalE,//332
      LHCb::ProtoParticle::additionalInfo::CaloHcalE,//333
      LHCb::ProtoParticle::additionalInfo::CaloEcalChi2,//334
      LHCb::ProtoParticle::additionalInfo::CaloBremChi2,//335
      LHCb::ProtoParticle::additionalInfo::CaloClusChi2,//336
      LHCb::ProtoParticle::additionalInfo::CaloNeutralPrsM,//343
      LHCb::ProtoParticle::additionalInfo::CaloShapeFr2r4,//344
      LHCb::ProtoParticle::additionalInfo::CaloShapeKappa,//345
      LHCb::ProtoParticle::additionalInfo::CaloShapeAsym,//346
      LHCb::ProtoParticle::additionalInfo::CaloShapeE1,//347
      LHCb::ProtoParticle::additionalInfo::CaloShapeE2,//348
      LHCb::ProtoParticle::additionalInfo::CaloPrsShapeE2,//349
      LHCb::ProtoParticle::additionalInfo::CaloPrsShapeEmax,//350
      LHCb::ProtoParticle::additionalInfo::CaloPrsShapeFr2,//351
      LHCb::ProtoParticle::additionalInfo::CaloPrsShapeAsym,//352
      LHCb::ProtoParticle::additionalInfo::CaloPrsM,//353
      LHCb::ProtoParticle::additionalInfo::CaloPrsM15,//354
      LHCb::ProtoParticle::additionalInfo::CaloPrsM30,//355
      LHCb::ProtoParticle::additionalInfo::CaloPrsM45,//356
      LHCb::ProtoParticle::additionalInfo::CaloClusterCode,//357
      LHCb::ProtoParticle::additionalInfo::CaloClusterFrac,//358
      LHCb::ProtoParticle::additionalInfo::CombDLLe,//600
      LHCb::ProtoParticle::additionalInfo::CombDLLmu,//601
      LHCb::ProtoParticle::additionalInfo::CombDLLpi,//602
      LHCb::ProtoParticle::additionalInfo::CombDLLk,//603
      LHCb::ProtoParticle::additionalInfo::CombDLLp,//604
      LHCb::ProtoParticle::additionalInfo::InAccBrem,// Extra for PID group
      LHCb::ProtoParticle::additionalInfo::InAccSpd,// ''
      LHCb::ProtoParticle::additionalInfo::InAccPrs,// ''
      LHCb::ProtoParticle::additionalInfo::InAccEcal,// ''
      LHCb::ProtoParticle::additionalInfo::InAccHcal,// ''
      LHCb::ProtoParticle::additionalInfo::VeloCharge,// ''
      LHCb::ProtoParticle::additionalInfo::RichPIDStatus,// ''
      LHCb::ProtoParticle::additionalInfo::CaloChargedID,//
      LHCb::ProtoParticle::additionalInfo::CaloChargedEcal,//
      LHCb::ProtoParticle::additionalInfo::CaloChargedPrs,//
      LHCb::ProtoParticle::additionalInfo::CaloChargedSpd,//
      LHCb::ProtoParticle::additionalInfo::ProbNNe,//
      LHCb::ProtoParticle::additionalInfo::ProbNNmu,//
      LHCb::ProtoParticle::additionalInfo::ProbNNpi,//
      LHCb::ProtoParticle::additionalInfo::ProbNNk,//
      LHCb::ProtoParticle::additionalInfo::ProbNNp,//
      LHCb::ProtoParticle::additionalInfo::ProbNNghost, //
      LHCb::ProtoParticle::additionalInfo::CombDLLd,//605
      LHCb::ProtoParticle::additionalInfo::MuonChi2Corr,//
      LHCb::ProtoParticle::additionalInfo::MuonMVA1,//
      LHCb::ProtoParticle::additionalInfo::MuonMVA2,//
      LHCb::ProtoParticle::additionalInfo::MuonMVA3,//
      LHCb::ProtoParticle::additionalInfo::MuonMVA4//
  };
    //===========================================================================
    /// Version unordered_map for LHCb::Particle in the Turbo stream
  static const unordered_map<int, unordered_map<string,pair<int,int>>> s_particle_unordered_map2_Turbo {
      { 3
        , {{"0#Particle.particleID.pid",                {0, 0}}
          , {"1#Particle.measuredMass",                 {1, 1}}
          , {"2#Particle.referencePoint.z",            {12, 2}}
          , {"3#Particle.referencePoint.x",            {23, 3}}
          , {"4#Particle.referencePoint.y",            {32, 4}}
          , {"5#Particle.slopes.x",                    {33, 5}}
          , {"6#Particle.slopes.y",                    {34, 6}}
          , {"7#Particle.1/p",                         {35, 7}}
          , {"8#Particle.conflevel",                   {36, 8}}
          , {"9#Particle.massErr",                     {37, 9}}
          , {"10#Particle.momCov00",                   {2, 10}}
          , {"11#Particle.momCov11",                   {3, 11}}
          , {"12#Particle.momCov22",                   {4, 12}}
          , {"13#Particle.momCov33",                    {5,13}}
          , {"14#Particle.momCov10",                    {6,14}}
          , {"15#Particle.momCov20",                    {7,15}}
          , {"16#Particle.momCov21",                    {8,16}}
          , {"17#Particle.momCov30",                    {9,17}}
          , {"18#Particle.momCov31",                   {10,18}}
          , {"19#Particle.momCov32",                   {11,19}}
          , {"20#Particle.posmomCov00",                {13,20}}
          , {"21#Particle.posmomCov11",                {14,21}}
          , {"22#Particle.posmomCov22",                {15,22}}
          , {"23#Particle.posmomCov10",                {16,23}}
          , {"24#Particle.posmomCov01",                {17,24}}
          , {"25#Particle.posmomCov20",                {18,25}}
          , {"26#Particle.posmomCov02",                {19,26}}
          , {"27#Particle.posmomCov21",                {20,27}}
          , {"28#Particle.posmomCov12",                {21,28}}
          , {"29#Particle.posmomCov30",                {22,29}}
          , {"30#Particle.posmomCov31",                {24,30}}
          , {"31#Particle.posmomCov32",                {25,31}}
          , {"32#Particle.posCov00",                   {26,32}}
          , {"33#Particle.posCov11",                   {27,33}}
          , {"34#Particle.posCov22",                   {28,34}}
          , {"35#Particle.posCov10",                   {29,35}}
          , {"36#Particle.posCov20",                   {30,36}}
          , {"37#Particle.posCov21",                   {31,37}}}
      }
      , { 4
        , {{"0#Particle.particleID.pid",                {0, 0}}
          , {"1#Particle.measuredMass",                 {1, 1}}
          , {"2#Particle.referencePoint.z",            {12, 2}}
          , {"3#Particle.referencePoint.x",            {23, 3}}
          , {"4#Particle.referencePoint.y",            {32, 4}}
          , {"5#Particle.slopes.x",                    {33, 5}}
          , {"6#Particle.slopes.y",                    {34, 6}}
          , {"7#Particle.1/p",                         {35, 7}}
          , {"8#Particle.conflevel",                   {36, 8}}
          , {"9#Particle.massErr",                     {37, 9}}
          , {"10#Particle.momCov00",                   {2, 10}}
          , {"11#Particle.momCov11",                   {3, 11}}
          , {"12#Particle.momCov22",                   {4, 12}}
          , {"13#Particle.momCov33",                    {5,13}}
          , {"14#Particle.momCov10",                    {6,14}}
          , {"15#Particle.momCov20",                    {7,15}}
          , {"16#Particle.momCov21",                    {8,16}}
          , {"17#Particle.momCov30",                    {9,17}}
          , {"18#Particle.momCov31",                   {10,18}}
          , {"19#Particle.momCov32",                   {11,19}}
          , {"20#Particle.posmomCov00",                {13,20}}
          , {"21#Particle.posmomCov11",                {14,21}}
          , {"22#Particle.posmomCov22",                {15,22}}
          , {"23#Particle.posmomCov10",                {16,23}}
          , {"24#Particle.posmomCov01",                {17,24}}
          , {"25#Particle.posmomCov20",                {18,25}}
          , {"26#Particle.posmomCov02",                {19,26}}
          , {"27#Particle.posmomCov21",                {20,27}}
          , {"28#Particle.posmomCov12",                {21,28}}
          , {"29#Particle.posmomCov30",                {22,29}}
          , {"30#Particle.posmomCov31",                {24,30}}
          , {"31#Particle.posmomCov32",                {25,31}}
          , {"32#Particle.posCov00",                   {26,32}}
          , {"33#Particle.posCov11",                   {27,33}}
          , {"34#Particle.posCov22",                   {28,34}}
          , {"35#Particle.posCov10",                   {29,35}}
          , {"36#Particle.posCov20",                   {30,36}}
          , {"37#Particle.posCov21",                   {31,37}}}
      }
      , { 6
        , {{"0#Particle.particleID.pid",                {0, 0}}
          , {"1#Particle.measuredMass",                 {1, 1}}
          , {"10#Particle.momCov00",                    {2,10}}
          , {"11#Particle.momCov11",                    {3,11}}
          , {"12#Particle.momCov22",                    {4,12}}
          , {"13#Particle.momCov33",                    {5,13}}
          , {"14#Particle.momCov10",                    {6,14}}
          , {"15#Particle.momCov20",                    {7,15}}
          , {"16#Particle.momCov21",                    {8,16}}
          , {"17#Particle.momCov30",                    {9,17}}
          , {"18#Particle.momCov31",                   {10,18}}
          , {"19#Particle.momCov32",                   {11,19}}
          , {"2#Particle.referencePoint.z",            {12, 2}}
          , {"20#Particle.posmomCov00",                {13,20}}
          , {"21#Particle.posmomCov11",                {14,21}}
          , {"22#Particle.posmomCov22",                {15,22}}
          , {"23#Particle.posmomCov10",                {16,23}}
          , {"24#Particle.posmomCov01",                {17,24}}
          , {"25#Particle.posmomCov20",                {18,25}}
          , {"26#Particle.posmomCov02",                {19,26}}
          , {"27#Particle.posmomCov21",                {20,27}}
          , {"28#Particle.posmomCov12",                {21,28}}
          , {"29#Particle.posmomCov30",                {22,29}}
          , {"3#Particle.referencePoint.x",            {23, 3}}
          , {"30#Particle.posmomCov31",                {24,30}}
          , {"31#Particle.posmomCov32",                {25,31}}
          , {"32#Particle.posCov00",                   {26,32}}
          , {"33#Particle.posCov11",                   {27,33}}
          , {"34#Particle.posCov22",                   {28,34}}
          , {"35#Particle.posCov10",                   {29,35}}
          , {"36#Particle.posCov20",                   {30,36}}
          , {"37#Particle.posCov21",                   {31,37}}
          , {"38#Particle.raw.m",                      {32,38}}
          , {"39#Particle.raw.p1",                     {33,39}}
          , {"4#Particle.referencePoint.y",            {34, 4}}
          , {"40#Particle.raw.p2",                     {35,40}}
          , {"41#Particle.raw.p3",                     {36,41}}
          , {"5#Particle.slopes.x",                    {37, 5}}
          , {"6#Particle.slopes.y",                    {38, 6}}
          , {"7#Particle.1/p",                         {39, 7}}
          , {"8#Particle.conflevel",                   {40, 8}}
          , {"9#Particle.massErr",                     {41, 9}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::Particle in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int>>> s_particle_unordered_map2 {
      { 1 , {{"0#Particle.particleID.pid",   {0,0}},
             {"1#Particle.measuredMass",     {1,1}},
             {"2#Particle.referencePoint.z", {2,2}},
             {"3#Particle.referencePoint.x", {3,3}},
             {"4#Particle.referencePoint.y", {4,4}},
             {"5#Particle.slopes.x",         {5,5}},
             {"6#Particle.slopes.y",         {6,6}},
             {"7#Particle.1/p",              {7,7}}} } ,

      { 2 , {{"0#Particle.particleID.pid",   {0,0}},
             {"1#Particle.measuredMass",     {1,1}},
             {"2#Particle.referencePoint.z", {2,2}},
             {"3#Particle.referencePoint.x", {3,3}},
             {"4#Particle.referencePoint.y", {4,4}},
             {"5#Particle.slopes.x",         {5,5}},
             {"6#Particle.slopes.y",         {6,6}},
             {"7#Particle.1/p",              {7,7}}} } ,

      { 3 , {{"0#Particle.particleID.pid",   {0,0}},
             {"1#Particle.measuredMass",     {1,1}},
             {"2#Particle.referencePoint.z", {2,2}},
             {"3#Particle.referencePoint.x", {3,3}},
             {"4#Particle.referencePoint.y", {4,4}},
             {"5#Particle.slopes.x",         {5,5}},
             {"6#Particle.slopes.y",         {6,6}},
             {"7#Particle.1/p",              {7,7}}} } ,

      { 4 , {{"0#Particle.particleID.pid",   {0,0}},
             {"1#Particle.measuredMass",     {1,1}},
             {"2#Particle.referencePoint.z", {2,2}},
             {"3#Particle.referencePoint.x", {3,3}},
             {"4#Particle.referencePoint.y", {4,4}},
             {"5#Particle.slopes.x",         {5,5}},
             {"6#Particle.slopes.y",         {6,6}},
             {"7#Particle.1/p",              {7,7}}} }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::ProtoParticle in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int>>> s_proto_unordered_map2_Turbo {
      { 3 , {{"0#Proto.extraInfo.IsPhoton",               {0,0}}}
      } ,
      { 4 , {{"0#Proto.extraInfo.IsPhoton",               { 0, 0}},
             {"1#Proto.extraInfo.IsNotE",                 { 1, 1}},
             {"2#Proto.extraInfo.IsNotH",                 {12, 2}},
             {"3#Proto.extraInfo.EcalPIDe",               {23, 3}},
             {"4#Proto.extraInfo.PrsPIDe",                {34, 4}},
             {"5#Proto.extraInfo.BremPIDe",               {45, 5}},
             {"6#Proto.extraInfo.HcalPIDe",               {51, 6}},
             {"7#Proto.extraInfo.HcalPIDmu",              {52, 7}},
             {"8#Proto.extraInfo.EcalPIDmu",              {53, 8}},
             {"9#Proto.extraInfo.CaloTrMatch",            {54, 9}},
             {"10#Proto.extraInfo.CaloElectronMatch",     { 2,10}},
             {"11#Proto.extraInfo.CaloBremMatch",         { 3,11}},
             {"12#Proto.extraInfo.CaloNeutralSpd",        { 4,12}},
             {"13#Proto.extraInfo.CaloNeutralPrs",        { 5,13}},
             {"14#Proto.extraInfo.CaloNeutralEcal",       { 6,14}},
             {"15#Proto.extraInfo.CaloNeutralHcal2Ecal",  { 7,15}},
             {"16#Proto.extraInfo.CaloNeutralE49",        { 8,16}},
             {"17#Proto.extraInfo.CaloNeutralID",         { 9,17}},
             {"18#Proto.extraInfo.CaloDepositID",         {10,18}},
             {"19#Proto.extraInfo.ShowerShape",           {11,19}},
             {"20#Proto.extraInfo.ClusterMass",           {13,20}},
             {"21#Proto.extraInfo.CaloSpdE",              {14,21}},
             {"22#Proto.extraInfo.CaloPrsE",              {15,22}},
             {"23#Proto.extraInfo.CaloEcalE",             {16,23}},
             {"24#Proto.extraInfo.CaloHcalE",             {17,24}},
             {"25#Proto.extraInfo.CaloEcalChi2",          {18,25}},
             {"26#Proto.extraInfo.CaloBremChi2",          {19,26}},
             {"27#Proto.extraInfo.CaloClusChi2",          {20,27}},
             {"28#Proto.extraInfo.CaloNeutralPrsM",       {21,28}},
             {"29#Proto.extraInfo.CaloShapeFr2r4",        {22,29}},
             {"30#Proto.extraInfo.CaloShapeKappa",        {24,30}},
             {"31#Proto.extraInfo.CaloShapeAsym",         {25,31}},
             {"32#Proto.extraInfo.CaloShapeE1",           {26,32}},
             {"33#Proto.extraInfo.CaloShapeE2",           {27,33}},
             {"34#Proto.extraInfo.CaloPrsShapeE2",        {28,34}},
             {"35#Proto.extraInfo.CaloPrsShapeEmax",      {29,35}},
             {"36#Proto.extraInfo.CaloPrsShapeFr2",       {30,36}},
             {"37#Proto.extraInfo.CaloPrsShapeAsym",      {31,37}},
             {"38#Proto.extraInfo.CaloPrsM",              {32,38}},
             {"39#Proto.extraInfo.CaloPrsM15",            {33,39}},
             {"40#Proto.extraInfo.CaloPrsM30",            {35,40}},
             {"41#Proto.extraInfo.CaloPrsM45",            {36,41}},
             {"42#Proto.extraInfo.CaloClusterCode",       {37,42}},
             {"43#Proto.extraInfo.CaloClusterFrac",       {38,43}},
             {"44#Proto.extraInfo.CombDLLe",              {39,44}},
             {"45#Proto.extraInfo.CombDLLmu",             {40,45}},
             {"46#Proto.extraInfo.CombDLLpi",             {41,46}},
             {"47#Proto.extraInfo.CombDLLk",              {42,47}},
             {"48#Proto.extraInfo.CombDLLp",              {43,48}},
             {"49#Proto.extraInfo.InAccBrem",             {44,49}},
             {"50#Proto.extraInfo.InAccSpd",              {46,50}},
             {"51#Proto.extraInfo.InAccPrs",              {47,51}},
             {"52#Proto.extraInfo.InAccEcal",             {48,52}},
             {"53#Proto.extraInfo.InAccHcal",             {49,53}},
             {"54#Proto.extraInfo.VeloCharge",            {50,54}}}
      } ,
      { 8 , {{"0#Proto.extraInfo.IsPhoton",               { 0, 0}},
             {"1#Proto.extraInfo.IsNotE",                 { 1, 1}},
             {"2#Proto.extraInfo.IsNotH",                 {12, 2}},
             {"3#Proto.extraInfo.EcalPIDe",               {23, 3}},
             {"4#Proto.extraInfo.PrsPIDe",                {34, 4}},
             {"5#Proto.extraInfo.BremPIDe",               {45, 5}},
             {"6#Proto.extraInfo.HcalPIDe",               {56, 6}},
             {"7#Proto.extraInfo.HcalPIDmu",              {57, 7}},
             {"8#Proto.extraInfo.EcalPIDmu",              {58, 8}},
             {"9#Proto.extraInfo.CaloTrMatch",            {59, 9}},
             {"10#Proto.extraInfo.CaloElectronMatch",     { 2,10}},
             {"11#Proto.extraInfo.CaloBremMatch",         { 3,11}},
             {"12#Proto.extraInfo.CaloNeutralSpd",        { 4,12}},
             {"13#Proto.extraInfo.CaloNeutralPrs",        { 5,13}},
             {"14#Proto.extraInfo.CaloNeutralEcal",       { 6,14}},
             {"15#Proto.extraInfo.CaloNeutralHcal2Ecal",  { 7,15}},
             {"16#Proto.extraInfo.CaloNeutralE49",        { 8,16}},
             {"17#Proto.extraInfo.CaloNeutralID",         { 9,17}},
             {"18#Proto.extraInfo.CaloDepositID",         {10,18}},
             {"19#Proto.extraInfo.ShowerShape",           {11,19}},
             {"20#Proto.extraInfo.ClusterMass",           {13,20}},
             {"21#Proto.extraInfo.CaloSpdE",              {14,21}},
             {"22#Proto.extraInfo.CaloPrsE",              {15,22}},
             {"23#Proto.extraInfo.CaloEcalE",             {16,23}},
             {"24#Proto.extraInfo.CaloHcalE",             {17,24}},
             {"25#Proto.extraInfo.CaloEcalChi2",          {18,25}},
             {"26#Proto.extraInfo.CaloBremChi2",          {19,26}},
             {"27#Proto.extraInfo.CaloClusChi2",          {20,27}},
             {"28#Proto.extraInfo.CaloNeutralPrsM",       {21,28}},
             {"29#Proto.extraInfo.CaloShapeFr2r4",        {22,29}},
             {"30#Proto.extraInfo.CaloShapeKappa",        {24,30}},
             {"31#Proto.extraInfo.CaloShapeAsym",         {25,31}},
             {"32#Proto.extraInfo.CaloShapeE1",           {26,32}},
             {"33#Proto.extraInfo.CaloShapeE2",           {27,33}},
             {"34#Proto.extraInfo.CaloPrsShapeE2",        {28,34}},
             {"35#Proto.extraInfo.CaloPrsShapeEmax",      {29,35}},
             {"36#Proto.extraInfo.CaloPrsShapeFr2",       {30,36}},
             {"37#Proto.extraInfo.CaloPrsShapeAsym",      {31,37}},
             {"38#Proto.extraInfo.CaloPrsM",              {32,38}},
             {"39#Proto.extraInfo.CaloPrsM15",            {33,39}},
             {"40#Proto.extraInfo.CaloPrsM30",            {35,40}},
             {"41#Proto.extraInfo.CaloPrsM45",            {36,41}},
             {"42#Proto.extraInfo.CaloClusterCode",       {37,42}},
             {"43#Proto.extraInfo.CaloClusterFrac",       {38,43}},
             {"44#Proto.extraInfo.CombDLLe",              {39,44}},
             {"45#Proto.extraInfo.CombDLLmu",             {40,45}},
             {"46#Proto.extraInfo.CombDLLpi",             {41,46}},
             {"47#Proto.extraInfo.CombDLLk",              {42,47}},
             {"48#Proto.extraInfo.CombDLLp",              {43,48}},
             {"49#Proto.extraInfo.InAccBrem",             {44,49}},
             {"50#Proto.extraInfo.InAccSpd",              {46,50}},
             {"51#Proto.extraInfo.InAccPrs",              {47,51}},
             {"52#Proto.extraInfo.InAccEcal",             {48,52}},
             {"53#Proto.extraInfo.InAccHcal",             {49,53}},
             {"54#Proto.extraInfo.VeloCharge",            {50,54}},
             {"55#Proto.extraInfo.RichPIDStatus",         {51,55}},
             {"56#Proto.extraInfo.CaloChargedID",         {52,56}},
             {"57#Proto.extraInfo.CaloChargedEcal",       {53,57}},
             {"58#Proto.extraInfo.CaloChargedPrs",        {54,58}},
             {"59#Proto.extraInfo.CaloChargedSpd",        {55,59}}}
      }
      , { 9
        , {{"0#Proto.extraInfo.IsPhoton",               { 0, 0}}
        ,  {"1#Proto.extraInfo.IsNotE",                 { 1, 1}}
        ,  {"2#Proto.extraInfo.IsNotH",                 {12, 2}}
        ,  {"3#Proto.extraInfo.EcalPIDe",               {23, 3}}
        ,  {"4#Proto.extraInfo.PrsPIDe",                {34, 4}}
        ,  {"5#Proto.extraInfo.BremPIDe",               {45, 5}}
        ,  {"6#Proto.extraInfo.HcalPIDe",               {55, 6}}
        ,  {"7#Proto.extraInfo.HcalPIDmu",              {62, 7}}
        ,  {"8#Proto.extraInfo.EcalPIDmu",              {63, 8}}
        ,  {"9#Proto.extraInfo.CaloTrMatch",            {64, 9}}
        ,  {"10#Proto.extraInfo.CaloElectronMatch",     { 2,10}}
        ,  {"11#Proto.extraInfo.CaloBremMatch",         { 3,11}}
        ,  {"12#Proto.extraInfo.CaloNeutralSpd",        { 4,12}}
        ,  {"13#Proto.extraInfo.CaloNeutralPrs",        { 5,13}}
        ,  {"14#Proto.extraInfo.CaloNeutralEcal",       { 6,14}}
        ,  {"15#Proto.extraInfo.CaloNeutralHcal2Ecal",  { 7,15}}
        ,  {"16#Proto.extraInfo.CaloNeutralE49",        { 8,16}}
        ,  {"17#Proto.extraInfo.CaloNeutralID",         { 9,17}}
        ,  {"18#Proto.extraInfo.CaloDepositID",         {10,18}}
        ,  {"19#Proto.extraInfo.ShowerShape",           {11,19}}
        ,  {"20#Proto.extraInfo.ClusterMass",           {13,20}}
        ,  {"21#Proto.extraInfo.CaloSpdE",              {14,21}}
        ,  {"22#Proto.extraInfo.CaloPrsE",              {15,22}}
        ,  {"23#Proto.extraInfo.CaloEcalE",             {16,23}}
        ,  {"24#Proto.extraInfo.CaloHcalE",             {17,24}}
        ,  {"25#Proto.extraInfo.CaloEcalChi2",          {18,25}}
        ,  {"26#Proto.extraInfo.CaloBremChi2",          {19,26}}
        ,  {"27#Proto.extraInfo.CaloClusChi2",          {20,27}}
        ,  {"28#Proto.extraInfo.CaloNeutralPrsM",       {21,28}}
        ,  {"29#Proto.extraInfo.CaloShapeFr2r4",        {22,29}}
        ,  {"30#Proto.extraInfo.CaloShapeKappa",        {24,30}}
        ,  {"31#Proto.extraInfo.CaloShapeAsym",         {25,31}}
        ,  {"32#Proto.extraInfo.CaloShapeE1",           {26,32}}
        ,  {"33#Proto.extraInfo.CaloShapeE2",           {27,33}}
        ,  {"34#Proto.extraInfo.CaloPrsShapeE2",        {28,34}}
        ,  {"35#Proto.extraInfo.CaloPrsShapeEmax",      {29,35}}
        ,  {"36#Proto.extraInfo.CaloPrsShapeFr2",       {30,36}}
        ,  {"37#Proto.extraInfo.CaloPrsShapeAsym",      {31,37}}
        ,  {"38#Proto.extraInfo.CaloPrsM",              {32,38}}
        ,  {"39#Proto.extraInfo.CaloPrsM15",            {33,39}}
        ,  {"40#Proto.extraInfo.CaloPrsM30",            {35,40}}
        ,  {"41#Proto.extraInfo.CaloPrsM45",            {36,41}}
        ,  {"42#Proto.extraInfo.CaloClusterCode",       {37,42}}
        ,  {"43#Proto.extraInfo.CaloClusterFrac",       {38,43}}
        ,  {"44#Proto.extraInfo.CombDLLe",              {39,44}}
        ,  {"45#Proto.extraInfo.CombDLLmu",             {40,45}}
        ,  {"46#Proto.extraInfo.CombDLLpi",             {41,46}}
        ,  {"47#Proto.extraInfo.CombDLLk",              {42,47}}
        ,  {"48#Proto.extraInfo.CombDLLp",              {43,48}}
        ,  {"49#Proto.extraInfo.InAccBrem",             {44,49}}
        ,  {"50#Proto.extraInfo.InAccSpd",              {46,50}}
        ,  {"51#Proto.extraInfo.InAccPrs",              {47,51}}
        ,  {"52#Proto.extraInfo.InAccEcal",             {48,52}}
        ,  {"53#Proto.extraInfo.InAccHcal",             {49,53}}
        ,  {"54#Proto.extraInfo.VeloCharge",            {50,54}}
        ,  {"56#Proto.extraInfo.CaloChargedID",         {51,56}}
        ,  {"57#Proto.extraInfo.CaloChargedEcal",       {52,57}}
        ,  {"58#Proto.extraInfo.CaloChargedPrs",        {53,58}}
        ,  {"59#Proto.extraInfo.CaloChargedSpd",        {54,59}}
        ,  {"60#Proto.extraInfo.ProbNNe",               {56,60}}
        ,  {"61#Proto.extraInfo.ProbNNmu",              {57,61}}
        ,  {"62#Proto.extraInfo.ProbNNpi",              {58,62}}
        ,  {"63#Proto.extraInfo.ProbNNk",               {59,63}}
        ,  {"64#Proto.extraInfo.ProbNNp",               {60,64}}
        ,  {"65#Proto.extraInfo.ProbNNghost",           {61,65}}}
      }
      , { 10
        , {{"0#Proto.extraInfo.IsPhoton",               { 0, 0}}
        ,  {"1#Proto.extraInfo.IsNotE",                 { 1, 1}}
        ,  {"2#Proto.extraInfo.IsNotH",                 {12, 2}}
        ,  {"3#Proto.extraInfo.EcalPIDe",               {23, 3}}
        ,  {"4#Proto.extraInfo.PrsPIDe",                {34, 4}}
        ,  {"5#Proto.extraInfo.BremPIDe",               {45, 5}}
        ,  {"6#Proto.extraInfo.HcalPIDe",               {55, 6}}
        ,  {"7#Proto.extraInfo.HcalPIDmu",              {63, 7}}
        ,  {"8#Proto.extraInfo.EcalPIDmu",              {64, 8}}
        ,  {"9#Proto.extraInfo.CaloTrMatch",            {65, 9}}
        ,  {"10#Proto.extraInfo.CaloElectronMatch",     { 2,10}}
        ,  {"11#Proto.extraInfo.CaloBremMatch",         { 3,11}}
        ,  {"12#Proto.extraInfo.CaloNeutralSpd",        { 4,12}}
        ,  {"13#Proto.extraInfo.CaloNeutralPrs",        { 5,13}}
        ,  {"14#Proto.extraInfo.CaloNeutralEcal",       { 6,14}}
        ,  {"15#Proto.extraInfo.CaloNeutralHcal2Ecal",  { 7,15}}
        ,  {"16#Proto.extraInfo.CaloNeutralE49",        { 8,16}}
        ,  {"17#Proto.extraInfo.CaloNeutralID",         { 9,17}}
        ,  {"18#Proto.extraInfo.CaloDepositID",         {10,18}}
        ,  {"19#Proto.extraInfo.ShowerShape",           {11,19}}
        ,  {"20#Proto.extraInfo.ClusterMass",           {13,20}}
        ,  {"21#Proto.extraInfo.CaloSpdE",              {14,21}}
        ,  {"22#Proto.extraInfo.CaloPrsE",              {15,22}}
        ,  {"23#Proto.extraInfo.CaloEcalE",             {16,23}}
        ,  {"24#Proto.extraInfo.CaloHcalE",             {17,24}}
        ,  {"25#Proto.extraInfo.CaloEcalChi2",          {18,25}}
        ,  {"26#Proto.extraInfo.CaloBremChi2",          {19,26}}
        ,  {"27#Proto.extraInfo.CaloClusChi2",          {20,27}}
        ,  {"28#Proto.extraInfo.CaloNeutralPrsM",       {21,28}}
        ,  {"29#Proto.extraInfo.CaloShapeFr2r4",        {22,29}}
        ,  {"30#Proto.extraInfo.CaloShapeKappa",        {24,30}}
        ,  {"31#Proto.extraInfo.CaloShapeAsym",         {25,31}}
        ,  {"32#Proto.extraInfo.CaloShapeE1",           {26,32}}
        ,  {"33#Proto.extraInfo.CaloShapeE2",           {27,33}}
        ,  {"34#Proto.extraInfo.CaloPrsShapeE2",        {28,34}}
        ,  {"35#Proto.extraInfo.CaloPrsShapeEmax",      {29,35}}
        ,  {"36#Proto.extraInfo.CaloPrsShapeFr2",       {30,36}}
        ,  {"37#Proto.extraInfo.CaloPrsShapeAsym",      {31,37}}
        ,  {"38#Proto.extraInfo.CaloPrsM",              {32,38}}
        ,  {"39#Proto.extraInfo.CaloPrsM15",            {33,39}}
        ,  {"40#Proto.extraInfo.CaloPrsM30",            {35,40}}
        ,  {"41#Proto.extraInfo.CaloPrsM45",            {36,41}}
        ,  {"42#Proto.extraInfo.CaloClusterCode",       {37,42}}
        ,  {"43#Proto.extraInfo.CaloClusterFrac",       {38,43}}
        ,  {"44#Proto.extraInfo.CombDLLe",              {39,44}}
        ,  {"45#Proto.extraInfo.CombDLLmu",             {40,45}}
        ,  {"46#Proto.extraInfo.CombDLLpi",             {41,46}}
        ,  {"47#Proto.extraInfo.CombDLLk",              {42,47}}
        ,  {"48#Proto.extraInfo.CombDLLp",              {43,48}}
        ,  {"49#Proto.extraInfo.InAccBrem",             {44,49}}
        ,  {"50#Proto.extraInfo.InAccSpd",              {46,50}}
        ,  {"51#Proto.extraInfo.InAccPrs",              {47,51}}
        ,  {"52#Proto.extraInfo.InAccEcal",             {48,52}}
        ,  {"53#Proto.extraInfo.InAccHcal",             {49,53}}
        ,  {"54#Proto.extraInfo.VeloCharge",            {50,54}}
        ,  {"56#Proto.extraInfo.CaloChargedID",         {51,56}}
        ,  {"57#Proto.extraInfo.CaloChargedEcal",       {52,57}}
        ,  {"58#Proto.extraInfo.CaloChargedPrs",        {53,58}}
        ,  {"59#Proto.extraInfo.CaloChargedSpd",        {54,59}}
        ,  {"60#Proto.extraInfo.ProbNNe",               {56,60}}
        ,  {"61#Proto.extraInfo.ProbNNmu",              {57,61}}
        ,  {"62#Proto.extraInfo.ProbNNpi",              {58,62}}
        ,  {"63#Proto.extraInfo.ProbNNk",               {59,63}}
        ,  {"64#Proto.extraInfo.ProbNNp",               {60,64}}
        ,  {"65#Proto.extraInfo.ProbNNghost",           {61,65}}
        ,  {"66#Proto.extraInfo.CombDLLd",              {62,66}}}
      }
      , { 11
        , {{"0#Proto.extraInfo.IsPhoton",               { 0, 0}}
        ,  {"1#Proto.extraInfo.IsNotE",                 { 1, 1}}
        ,  {"10#Proto.extraInfo.CaloElectronMatch",     { 2,10}}
        ,  {"11#Proto.extraInfo.CaloBremMatch",         { 3,11}}
        ,  {"12#Proto.extraInfo.CaloNeutralSpd",        { 4,12}}
        ,  {"13#Proto.extraInfo.CaloNeutralPrs",        { 5,13}}
        ,  {"14#Proto.extraInfo.CaloNeutralEcal",       { 6,14}}
        ,  {"15#Proto.extraInfo.CaloNeutralHcal2Ecal",  { 7,15}}
        ,  {"16#Proto.extraInfo.CaloNeutralE49",        { 8,16}}
        ,  {"17#Proto.extraInfo.CaloNeutralID",         { 9,17}}
        ,  {"18#Proto.extraInfo.CaloDepositID",         {10,18}}
        ,  {"19#Proto.extraInfo.ShowerShape",           {11,19}}
        ,  {"2#Proto.extraInfo.IsNotH",                 {12, 2}}
        ,  {"20#Proto.extraInfo.ClusterMass",           {13,20}}
        ,  {"21#Proto.extraInfo.CaloSpdE",              {14,21}}
        ,  {"22#Proto.extraInfo.CaloPrsE",              {15,22}}
        ,  {"23#Proto.extraInfo.CaloEcalE",             {16,23}}
        ,  {"24#Proto.extraInfo.CaloHcalE",             {17,24}}
        ,  {"25#Proto.extraInfo.CaloEcalChi2",          {18,25}}
        ,  {"26#Proto.extraInfo.CaloBremChi2",          {19,26}}
        ,  {"27#Proto.extraInfo.CaloClusChi2",          {20,27}}
        ,  {"28#Proto.extraInfo.CaloNeutralPrsM",       {21,28}}
        ,  {"29#Proto.extraInfo.CaloShapeFr2r4",        {22,29}}
        ,  {"3#Proto.extraInfo.EcalPIDe",               {23, 3}}
        ,  {"30#Proto.extraInfo.CaloShapeKappa",        {24,30}}
        ,  {"31#Proto.extraInfo.CaloShapeAsym",         {25,31}}
        ,  {"32#Proto.extraInfo.CaloShapeE1",           {26,32}}
        ,  {"33#Proto.extraInfo.CaloShapeE2",           {27,33}}
        ,  {"34#Proto.extraInfo.CaloPrsShapeE2",        {28,34}}
        ,  {"35#Proto.extraInfo.CaloPrsShapeEmax",      {29,35}}
        ,  {"36#Proto.extraInfo.CaloPrsShapeFr2",       {30,36}}
        ,  {"37#Proto.extraInfo.CaloPrsShapeAsym",      {31,37}}
        ,  {"38#Proto.extraInfo.CaloPrsM",              {32,38}}
        ,  {"39#Proto.extraInfo.CaloPrsM15",            {33,39}}
        ,  {"4#Proto.extraInfo.PrsPIDe",                {34, 4}}
        ,  {"40#Proto.extraInfo.CaloPrsM30",            {35,40}}
        ,  {"41#Proto.extraInfo.CaloPrsM45",            {36,41}}
        ,  {"42#Proto.extraInfo.CaloClusterCode",       {37,42}}
        ,  {"43#Proto.extraInfo.CaloClusterFrac",       {38,43}}
        ,  {"44#Proto.extraInfo.CombDLLe",              {39,44}}
        ,  {"45#Proto.extraInfo.CombDLLmu",             {40,45}}
        ,  {"46#Proto.extraInfo.CombDLLpi",             {41,46}}
        ,  {"47#Proto.extraInfo.CombDLLk",              {42,47}}
        ,  {"48#Proto.extraInfo.CombDLLp",              {43,48}}
        ,  {"49#Proto.extraInfo.InAccBrem",             {44,49}}
        ,  {"5#Proto.extraInfo.BremPIDe",               {45, 5}}
        ,  {"50#Proto.extraInfo.InAccSpd",              {46,50}}
        ,  {"51#Proto.extraInfo.InAccPrs",              {47,51}}
        ,  {"52#Proto.extraInfo.InAccEcal",             {48,52}}
        ,  {"53#Proto.extraInfo.InAccHcal",             {49,53}}
        ,  {"54#Proto.extraInfo.VeloCharge",            {50,54}}
        ,  {"56#Proto.extraInfo.CaloChargedID",         {51,56}}
        ,  {"57#Proto.extraInfo.CaloChargedEcal",       {52,57}}
        ,  {"58#Proto.extraInfo.CaloChargedPrs",        {53,58}}
        ,  {"59#Proto.extraInfo.CaloChargedSpd",        {54,59}}
        ,  {"6#Proto.extraInfo.HcalPIDe",               {55, 6}}
        ,  {"60#Proto.extraInfo.ProbNNe",               {56,60}}
        ,  {"61#Proto.extraInfo.ProbNNmu",              {57,61}}
        ,  {"62#Proto.extraInfo.ProbNNpi",              {58,62}}
        ,  {"63#Proto.extraInfo.ProbNNk",               {59,63}}
        ,  {"64#Proto.extraInfo.ProbNNp",               {60,64}}
        ,  {"65#Proto.extraInfo.ProbNNghost",           {61,65}}
        ,  {"66#Proto.extraInfo.CombDLLd",              {62,66}}
        ,  {"67#Proto.extraInfo.MuonChi2Corr",          {63,67}}
        ,  {"68#Proto.extraInfo.MuonMVA1",              {64,68}}
        ,  {"69#Proto.extraInfo.MuonMVA2",              {65,69}}
        ,  {"7#Proto.extraInfo.HcalPIDmu",              {66, 7}}
        ,  {"70#Proto.extraInfo.MuonMVA3",              {67,70}}
        ,  {"71#Proto.extraInfo.MuonMVA4",              {68,71}}
        ,  {"8#Proto.extraInfo.EcalPIDmu",              {69, 8}}
        ,  {"9#Proto.extraInfo.CaloTrMatch",            {70, 9}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::ProtoParticle in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int>>> s_proto_unordered_map2 {
      { 4
        , {{"0#Proto.extraInfo.CombDLLe",              {0,44}}
        ,  {"1#Proto.extraInfo.CombDLLmu",             {1,45}}
        ,  {"2#Proto.extraInfo.CombDLLpi",             {2,46}}
        ,  {"3#Proto.extraInfo.CombDLLk",              {3,47}}
        ,  {"4#Proto.extraInfo.CombDLLp",              {4,48}}
        ,  {"5#Proto.extraInfo.RichPIDStatus",         {5,55}}}
      }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::Track for the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_track_unordered_map2_Turbo {
      { 3 , {{"0#Track.firstState.z",           { 0, 0}},
             {"1#Track.firstState.x",           { 1, 1}},
             {"2#Track.firstState.y",           { 9, 2}},
             {"3#Track.firstState.tx",          {10, 3}},
             {"4#Track.firstState.ty",          {11, 4}},
             {"5#Track.firstState.qOverP",      {12, 5}},
             {"6#Track.chi2PerDoF",             {13, 6}},
             {"7#Track.nDoF",                   {14, 7}},
             {"8#Track.Likelihood",             {15, 8}},
             {"9#Track.GhostProb",              {16, 9}},
             {"10#Track.flags",                 { 2,10}},
             {"11#Track.lastState.z",           { 3,11}},
             {"12#Track.lastState.x",           { 4,12}},
             {"13#Track.lastState.y",           { 5,13}},
             {"14#Track.lastState.tx",          { 6,14}},
             {"15#Track.lastState.ty",          { 7,15}},
             {"16#Track.lastState.qOverP",      { 8,16}}} },

      { 4 , {{"0#Track.firstState.z",           { 0, 0}},
             {"1#Track.firstState.x",           { 1, 1}},
             {"2#Track.firstState.y",           {12, 2}},
             {"3#Track.firstState.tx",          {16, 3}},
             {"4#Track.firstState.ty",          {17, 4}},
             {"5#Track.firstState.qOverP",      {18, 5}},
             {"6#Track.chi2PerDoF",             {19, 6}},
             {"7#Track.nDoF",                   {20, 7}},
             {"8#Track.Likelihood",             {21, 8}},
             {"9#Track.GhostProb",              {22, 9}},
             {"10#Track.flags",                 { 2,10}},
             {"11#Track.lastState.z",           { 3,11}},
             {"12#Track.lastState.x",           { 4,12}},
             {"13#Track.lastState.y",           { 5,13}},
             {"14#Track.lastState.tx",          { 6,14}},
             {"15#Track.lastState.ty",          { 7,15}},
             {"16#Track.lastState.qOverP",      { 8,16}},
             {"17#Track.CloneDist",             { 9,17}},
             {"18#Track.FitMatchChi2",          {10,18}},
             {"19#Track.FitVeloChi2",           {11,19}},
             {"20#Track.FitTChi2",              {13,20}},
             {"21#Track.FitVeloNDoF",           {14,21}},
             {"22#Track.FitTNDoF",              {15,22}}} },

      { 5 , {{"0#Track.firstState.z",           { 0, 0}},
             {"1#Track.firstState.x",           { 1, 1}},
             {"10#Track.flags",                 { 2,10}},
             {"11#Track.lastState.z",           { 3,11}},
             {"12#Track.lastState.x",           { 4,12}},
             {"13#Track.lastState.y",           { 5,13}},
             {"14#Track.lastState.tx",          { 6,14}},
             {"15#Track.lastState.ty",          { 7,15}},
             {"16#Track.lastState.qOverP",      { 8,16}},
             {"17#Track.CloneDist",             { 9,17}},
             {"18#Track.FitMatchChi2",          {10,18}},
             {"19#Track.FitVeloChi2",           {11,19}},
             {"2#Track.firstState.y",           {12, 2}},
             {"20#Track.FitTChi2",              {13,20}},
             {"21#Track.FitVeloNDoF",           {14,21}},
             {"22#Track.FitTNDoF",              {15,22}},
             {"23#Track.firstStateFlags",       {16,23}},
             {"24#Track.lastStateFlags",        {17,24}},
             {"25#Track.firstStateCov00",       {18,25}},
             {"26#Track.firstStateCov11",       {19,26}},
             {"27#Track.firstStateCov22",       {20,27}},
             {"28#Track.firstStateCov33",       {21,28}},
             {"29#Track.firstStateCov44",       {22,29}},
             {"3#Track.firstState.tx",          {23, 3}},
             {"30#Track.firstStateCov01",       {24,30}},
             {"31#Track.firstStateCov02",       {25,31}},
             {"32#Track.firstStateCov03",       {26,32}},
             {"33#Track.firstStateCov04",       {27,33}},
             {"34#Track.firstStateCov12",       {28,34}},
             {"35#Track.firstStateCov13",       {29,35}},
             {"36#Track.firstStateCov14",       {30,36}},
             {"37#Track.firstStateCov23",       {31,37}},
             {"38#Track.firstStateCov24",       {32,38}},
             {"39#Track.firstStateCov34",       {33,39}},
             {"4#Track.firstState.ty",          {34, 4}},
             {"5#Track.firstState.qOverP",      {35, 5}},
             {"6#Track.chi2PerDoF",             {36, 6}},
             {"7#Track.nDoF",                   {37, 7}},
             {"8#Track.Likelihood",             {38, 8}},
             {"9#Track.GhostProb",              {39, 9}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::Track
    static const unordered_map<int, unordered_map<string,pair<int, int> > > s_track_unordered_map2 {
      { 1 , { {"0#Track.firstState.z",            {0,0}},
              {"1#Track.firstState.x",            {1,1}},
              {"2#Track.firstState.y",            {2,2}},
              {"3#Track.firstState.tx",           {3,3}},
              {"4#Track.firstState.ty",           {4,4}},
              {"5#Track.firstState.qOverP",       {5,5}} } } ,

      { 2 , { {"0#Track.firstState.z",            {0,0}},
              {"1#Track.firstState.x",            {1,1}},
              {"2#Track.firstState.y",            {2,2}},
              {"3#Track.firstState.tx",           {3,3}},
              {"4#Track.firstState.ty",           {4,4}},
              {"5#Track.firstState.qOverP",       {5,5}},
              {"6#Track.chi2PerDoF",              {6,6}},
              {"7#Track.nDoF",                    {7,7}} } } ,

      { 3 , { {"0#Track.firstState.z",            {0,0}},
              {"1#Track.firstState.x",            {1,1}},
              {"2#Track.firstState.y",            {2,2}},
              {"3#Track.firstState.tx",           {3,3}},
              {"4#Track.firstState.ty",           {4,4}},
              {"5#Track.firstState.qOverP",       {5,5}},
              {"6#Track.chi2PerDoF",              {6,6}},
              {"7#Track.nDoF",                    {7,7}} } } ,

      { 4 , { {"0#Track.firstState.z",            {0,0}},
              {"1#Track.firstState.x",            {1,1}},
              {"2#Track.firstState.y",            {2,2}},
              {"3#Track.firstState.tx",           {3,3}},
              {"4#Track.firstState.ty",           {4,4}},
              {"5#Track.firstState.qOverP",       {5,5}},
              {"6#Track.chi2PerDoF",              {6,6}},
              {"7#Track.nDoF",                    {7,7}} } }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::RichPID in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_rpid_unordered_map2_Turbo {
      {  3 , { {"0#Rich.pidResultCode",            {0,0}},
               {"1#Rich.DLLe",                     {1,1}},
               {"2#Rich.DLLmu",                    {2,2}},
               {"3#Rich.DLLpi",                    {3,3}},
               {"4#Rich.DLLK",                     {4,4}},
               {"5#Rich.DLLp",                     {5,5}}} } ,

      {  4 , { {"0#Rich.pidResultCode",            {0,0}},
               {"1#Rich.DLLe",                     {1,1}},
               {"2#Rich.DLLmu",                    {2,2}},
               {"3#Rich.DLLpi",                    {3,3}},
               {"4#Rich.DLLK",                     {4,4}},
               {"5#Rich.DLLp",                     {5,5}},
               {"6#Rich.BelowThreshold",           {6,6}}} } ,

      { 10 , { {"0#Rich.pidResultCode",            {0,0}},
               {"1#Rich.DLLe",                     {1,1}},
               {"2#Rich.DLLmu",                    {2,2}},
               {"3#Rich.DLLpi",                    {3,3}},
               {"4#Rich.DLLK",                     {4,4}},
               {"5#Rich.DLLp",                     {5,5}},
               {"6#Rich.BelowThreshold",           {6,6}},
               {"7#Rich.DLLd",                     {7,7}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::RichPID in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_rpid_unordered_map2 {
    };

    //===========================================================================
    /// Version unordered_map for LHCb::MuonPID in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_mpid_unordered_map2_Turbo {
      { 3
        , {{"0#Muon.MuonLLMu",                  {0,0}}
          , {"1#Muon.MuonLLBg",                 {1,1}}
          , {"2#Muon.NShared",                  {2,2}}
          , {"3#Muon.Status",                   {3,3}}
          , {"4#Muon.IsMuon",                   {4,4}}
          , {"5#Muon.IsMuonLoose",              {5,5}}
          , {"6#Muon.IsMuonTight",              {6,6}}}
      }
      , { 4
        , {{"0#Muon.MuonLLMu",                  {0,0}}
          , {"1#Muon.MuonLLBg",                 {1,1}}
          , {"2#Muon.NShared",                  {2,2}}
          , {"3#Muon.Status",                   {3,3}}
          , {"4#Muon.IsMuon",                   {4,4}}
          , {"5#Muon.IsMuonLoose",              {5,5}}
          , {"6#Muon.IsMuonTight",              {6,6}}}
      }
      , { 11
        , {{"0#Muon.MuonLLMu",                  { 0, 0}}
          , {"1#Muon.MuonLLBg",                 { 1, 1}}
          , {"10#Muon.muonMVA3",                { 2,10}}
          , {"11#Muon.muonMVA4",                { 3,11}}
          , {"2#Muon.NShared",                  { 4, 2}}
          , {"3#Muon.Status",                   { 5, 3}}
          , {"4#Muon.IsMuon",                   { 6, 4}}
          , {"5#Muon.IsMuonLoose",              { 7, 5}}
          , {"6#Muon.IsMuonTight",              { 8, 6}}
          , {"7#Muon.chi2Corr",                 { 9, 7}}
          , {"8#Muon.muonMVA1",                 {10, 8}}
          , {"9#Muon.muonMVA2",                 {11, 9}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::MuonPID in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_mpid_unordered_map2 {
      { 4
        , {{"4#Muon.IsMuon",                   {0,4}}}
      }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::CaloCluster in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_calo_unordered_map2_Turbo {
      { 3
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
      , { 4
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::CaloCluster in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_calo_unordered_map2 {
      { 1
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
      , { 2
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
      , { 3
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
      , { 4
        , {{"0#CaloCluster.e",                        {0,0}}
          , {"1#CaloCluster.position.x",              {1,1}}
          , {"2#CaloCluster.position.y",              {2,2}}
          , {"3#CaloCluster.position.z",              {3,3}}}
      }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::CaloHypo in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_calohypo_unordered_map2_Turbo {
      { 8
        , {{"0#CaloHypo.e",                        {0,0}}
          , {"1#CaloHypo.position.x",              {1,1}}
          , {"2#CaloHypo.position.y",              {2,2}}
          , {"3#CaloHypo.position.z",              {3,3}}
          , {"4#CaloHypo.lh",                      {4,4}}
          , {"5#CaloHypo.h",                       {5,5}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::CaloHypo in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_calohypo_unordered_map2 {
    };

    //===========================================================================
    /// Version unordered_map for LHCb::RecVertex in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_recvertex_unordered_map2_Turbo {
      { 3
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {2,2}}
          , {"3#RecVertex.chi2",                      {3,3}}}
      }
      , { 4
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {4,2}}
          , {"3#RecVertex.chi2",                      {5,3}}
          , {"4#RecVertex.ndf",                       {6,4}}
          , {"5#RecVertex.technique",                 {7,5}}
          , {"6#RecVertex.cov00",                     {8,6}}
          , {"7#RecVertex.cov11",                     {9,7}}
          , {"8#RecVertex.cov22",                     {10,8}}
          , {"9#RecVertex.cov10",                     {11,9}}
          , {"10#RecVertex.cov20",                    {2,10}}
          , {"11#RecVertex.cov21",                    {3,11}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::RecVertex in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_recvertex_unordered_map2 {
      { 1
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {2,2}}}
      }
      , { 2
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {2,2}}
          , {"3#RecVertex.chi2",                      {3,3}}}
      }
      , { 3
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {2,2}}
          , {"3#RecVertex.chi2",                      {3,3}}}
      }
      , { 4
        , {{"0#RecVertex.position.x",                 {0,0}}
          , {"1#RecVertex.position.y",                {1,1}}
          , {"2#RecVertex.position.z",                {2,2}}
          , {"3#RecVertex.chi2",                      {3,3}}}
      }
    };

    //===========================================================================
    /// Version unordered_map for LHCb::Vertex in the Turbo stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_vertex_unordered_map2_Turbo {
      { 3
        , {{"0#Vertex.chi2",                {0,0}}
          , {"1#Vertex.ndf",                {1,1}}
          , {"2#Vertex.position.x",         {4,2}}
          , {"3#Vertex.position.y",         {5,3}}
          , {"4#Vertex.position.z",         {6,4}}
          , {"5#Vertex.technique",          {7,5}}
          , {"6#Vertex.cov00",              {8,6}}
          , {"7#Vertex.cov11",              {9,7}}
          , {"8#Vertex.cov22",             {10,8}}
          , {"9#Vertex.cov10",             {11,9}}
          , {"10#Vertex.cov20",             {2,10}}
          , {"11#Vertex.cov21",             {3,11}}}
      }
      , { 4
        , {{"0#Vertex.chi2",               { 0, 0}}
          , {"1#Vertex.ndf",               { 1, 1}}
          , {"10#Vertex.cov20",            { 2,10}}
          , {"11#Vertex.cov21",            { 3,11}}
          , {"2#Vertex.position.x",        { 4, 2}}
          , {"3#Vertex.position.y",        { 5, 3}}
          , {"4#Vertex.position.z",        { 6, 4}}
          , {"5#Vertex.technique",         { 7, 5}}
          , {"6#Vertex.cov00",             { 8, 6}}
          , {"7#Vertex.cov11",             { 9, 7}}
          , {"8#Vertex.cov22",             {10, 8}}
          , {"9#Vertex.cov10",             {11, 9}}}
      }
    };
    //===========================================================================
    /// Version unordered_map for LHCb::Vertex in the Full stream
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_vertex_unordered_map2 {
    };

    //===========================================================================
    /// Version unordered_map for LHCb::RecSummary
    static const unordered_map<int, unordered_map<string,pair<int,int> > > s_recsummary_unordered_map2 {
      { 4
        , {{"0#RecSummary.nLongTracks",                {0,0}}
          , {"1#RecSummary.nDownstreamTracks",         {1,1}}
          , {"2#RecSummary.nUpstreamTracks",           {12,2}}
          , {"3#RecSummary.nVeloTracks",               {15,3}}
          , {"4#RecSummary.nTTracks",                  {16,4}}
          , {"5#RecSummary.nBackTracks",               {17,5}}
          , {"6#RecSummary.nTracks",                   {18,6}}
          , {"7#RecSummary.nRich1Hits",                {19,7}}
          , {"8#RecSummary.nRich2Hits",                {20,8}}
          , {"9#RecSummary.nVeloClusters",             {21,9}}
          , {"10#RecSummary.nITClusters",              {2,10}}
          , {"11#RecSummary.nTTClusters",              {3,11}}
          , {"12#RecSummary.nUTClusters",              {4,12}}
          , {"13#RecSummary.nOTClusters",              {5,13}}
          , {"14#RecSummary.nFTClusters",              {6,14}}
          , {"15#RecSummary.nSPDhits",                 {7,15}}
          , {"16#RecSummary.nMuonCoordsS0",            {8,16}}
          , {"17#RecSummary.nMuonCoordsS1",            {9,17}}
          , {"18#RecSummary.nMuonCoordsS2",            {10,18}}
          , {"19#RecSummary.nMuonCoordsS3",            {11,19}}
          , {"20#RecSummary.nMuonCoordsS4",            {13,20}}
          , {"21#RecSummary.nMuonTracks",              {14,21}}}
      }
      , { 6
        , {{"0#RecSummary.nLongTracks",                { 0, 0}}
          , {"1#RecSummary.nDownstreamTracks",         { 1, 1}}
          , {"10#RecSummary.nITClusters",              { 2,10}}
          , {"11#RecSummary.nTTClusters",              { 3,11}}
          , {"12#RecSummary.nUTClusters",              { 4,12}}
          , {"13#RecSummary.nOTClusters",              { 5,13}}
          , {"14#RecSummary.nFTClusters",              { 6,14}}
          , {"15#RecSummary.nSPDhits",                 { 7,15}}
          , {"16#RecSummary.nMuonCoordsS0",            { 8,16}}
          , {"17#RecSummary.nMuonCoordsS1",            { 9,17}}
          , {"18#RecSummary.nMuonCoordsS2",            {10,18}}
          , {"19#RecSummary.nMuonCoordsS3",            {11,19}}
          , {"2#RecSummary.nUpstreamTracks",           {12, 2}}
          , {"20#RecSummary.nMuonCoordsS4",            {13,20}}
          , {"21#RecSummary.nMuonTracks",              {14,21}}
          , {"22#RecSummary.nPVs",                     {15,22}}
          , {"3#RecSummary.nVeloTracks",               {16, 3}}
          , {"4#RecSummary.nTTracks",                  {17, 4}}
          , {"5#RecSummary.nBackTracks",               {18, 5}}
          , {"6#RecSummary.nTracks",                   {19, 6}}
          , {"7#RecSummary.nRich1Hits",                {20, 7}}
          , {"8#RecSummary.nRich2Hits",                {21, 8}}
          , {"9#RecSummary.nVeloClusters",             {22, 9}}}
      }
    };
}

//-----------------------------------------------------------------------------
// Implementation file for class : ReportConvertTool: ReportConvertTool.cpp
// Author: Sean Benson
// 05/06/2008
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( ReportConvertTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ReportConvertTool::ReportConvertTool( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : base_class( type, name , parent )
{
  auto maps = { s_track_unordered_map2_Turbo,
                s_particle_unordered_map2_Turbo,
                s_proto_unordered_map2_Turbo,
                s_rpid_unordered_map2_Turbo,
                s_mpid_unordered_map2_Turbo,
                s_recvertex_unordered_map2_Turbo,
                s_vertex_unordered_map2_Turbo,
                s_recsummary_unordered_map2,
                s_calohypo_unordered_map2_Turbo,
                s_particle_unordered_map2,
                s_proto_unordered_map2,
                s_track_unordered_map2,
                s_rpid_unordered_map2,
                s_mpid_unordered_map2,
                s_calo_unordered_map2_Turbo,
                s_calo_unordered_map2,
                s_calohypo_unordered_map2,
                s_recvertex_unordered_map2,
                s_vertex_unordered_map2
  };

  for (const auto& map : maps ) {
    for(const auto & elem : map) {
        if(elem.first > m_LatestVersion) m_LatestVersion = elem.first;
    }
  }

  // Do some sanity checking on the maps.
  for(const auto& map : maps ) {
    for(const auto& elem : map ) {
      // elem.first is the version number
      // elem.second is { "int2#..." : (int1, int2) }
      // and int1 should be in ascending order when the keys are sorted
      // lexigraphically
      std::vector<std::pair<std::string, unsigned int> > check_int1;
      for(const auto& elem2 : elem.second) {
        auto hash_index = elem2.first.find_first_of('#');
        auto dot_index = elem2.first.find_first_of('.');
        auto last_dot_index = elem2.first.find_last_of('.');
        auto extrainfo_name = elem2.first.substr(last_dot_index+1);
        auto name_stem = elem2.first.substr(hash_index + 1,
            dot_index - hash_index - 1);
        auto int_string = elem2.first.substr(0, hash_index);
        auto int_from_string = std::stoi(int_string);
        if(name_stem == "Proto") {
          // key is e.g. 59#Proto.extraInfo.CaloChargedSpd
          auto extrainfo_enum = LHCb::ProtoParticle::convertExtraInfo(extrainfo_name);
          if(extrainfo_enum != s_proto_map[elem2.second.second]) {
            throw GaudiException("Incorrect entry in lookup map version "
                + std::to_string(elem.first) + ". Key " + elem2.first
                + " maps to (" + std::to_string(elem2.second.first) + ", "
                + std::to_string(elem2.second.second) + ") but the enum"
                + " value for " + extrainfo_name + " is "
                + std::to_string(extrainfo_enum) + ".",
                this->name(), StatusCode::FAILURE);
          }
        } else if(name_stem == "RecSummary") {
          auto recsummary_enum = LHCb::RecSummary::DataTypesToType(extrainfo_name);
          if(recsummary_enum != s_rsum_map[elem2.second.second]) {
            throw GaudiException("Incorrect entry in lookup map version "
                + std::to_string(elem.first) + ". Key " + elem2.first
                + " maps to (" + std::to_string(elem2.second.first) + ", "
                + std::to_string(elem2.second.second) + " = "
                + LHCb::RecSummary::DataTypesToString(elem2.second.second)
                + ") but the enum value for " + extrainfo_name + " is "
                + std::to_string(recsummary_enum) + ".",
                this->name(), StatusCode::FAILURE);
          }
        } else if(int_from_string != elem2.second.second) {
          throw GaudiException("Incorrect entry in lookup map version "
              + std::to_string(elem.first) + ". Key " + elem2.first
              + " maps to (" + std::to_string(elem2.second.first) + ", "
              + std::to_string(elem2.second.second)
              + ") but the integer part of the key is "
              + std::to_string(int_from_string) + ".",
              this->name(), StatusCode::FAILURE);
        }
        check_int1.emplace_back(elem2.first, elem2.second.first);
      }
      std::sort(check_int1.begin(), check_int1.end());
      for(auto i = 0u; i < check_int1.size(); i++) {
        if(i != check_int1[i].second) {
          throw GaudiException("Incorrect entry in lookup map version "
              + std::to_string(elem.first) + ". Key "
              + check_int1[i].first + " maps to ("
              + std::to_string(check_int1[i].second) + ", "
              + std::to_string(elem.second.at(check_int1[i].first).second)
              + ") but has index "
              + std::to_string(i) + " when sorted lexigraphically.",
              this->name(), StatusCode::FAILURE);
        }
      }
    }
  }
}


void ReportConvertTool::setReportVersion(int version){
  m_version = version;
}

void ReportConvertTool::setReportVersionLatest(){
  m_version = m_LatestVersion;
}

int ReportConvertTool::getReportVersion(){
  return m_version;
}

int ReportConvertTool::getLatestVersion(){
  return m_LatestVersion;
}

int ReportConvertTool::getSizeSelRepParticleLatest(){
  return s_particle_unordered_map2.at(findBestPrevious(s_particle_unordered_map2,m_LatestVersion)).size();
}

int ReportConvertTool::findBestPrevious(const unordered_map<int, unordered_map<string,pair<int,int> > >& map,int in) const{
  int out=in;
  for(int m=in;m>0;--m){
    auto it = map.find(m);
    out = m;
    if (it != map.end()) break;
  }
  return out;
}

void ReportConvertTool::SummaryFromRaw(HltObjectSummary::Info* info,
    HltSelRepRBStdInfo::StdInfo* subbank, int classID)
{
  // Version number that we use to ensure backwards compatibility with Run 1
  // (has nothing to do with header ID).
  int run1version=-999;

  if(UNLIKELY(msgLevel(MSG::DEBUG)))
    debug() << "SummaryFromRaw call for ID:" << classID << endmsg;

  // Make sure the tool is given a version to use
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  if(UNLIKELY(msgLevel(MSG::DEBUG)))
    debug() << "Assigning used_map with version:" << m_version << endmsg;

  // Which one of the member maps do we need to use?
  /*
   * Word on versions:
   * - Version 3 and greater for Run 2
   * - Versions 1 and 2 for Run 1
   * v3 is the first version in the header ID to have turbo information.
   * v1 and v2 in the header ID have nothing to do with level of information in the reports.
   * If (v1 || v2), Track, Particle, RecVertex and CaloCluster need to look for the level of information.
   * - Need to protect against someone with v1 or v2 asking for a Turbo map.
   * - Need to protect against v1 or v2 requesting a Turbo object decode (i.e. RichPID, etc.)
   */

  if((m_version<3 && m_version>0) && ( (classID!=LHCb::CLID_Track) && (classID!=LHCb::CLID_Particle) && (classID!=LHCb::CLID_RecVertex) && (classID!=LHCb::CLID_CaloCluster) ) )
      Error( std::string{ "Asked to decode Run 1 style reports with Turbo object (" } + std::to_string(classID) + std::string{ "). Please update your software" }, StatusCode::FAILURE, 100 ).ignore();

  auto pick_map = [&](const auto& map1, const auto& map2 ) {
        return  subbank->size()< map1.at( findBestPrevious( map1, m_version ) ).size() ?
                 &map2 : &map1;
  };
  const unordered_map<int,unordered_map<string, pair<int,int> > > *used_map = nullptr;
  switch( classID )
  {
    case LHCb::CLID_Track:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (s_track_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (s_track_unordered_map2.at(2)).size() ) run1version = 2;
          else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 ).ignore();
          used_map = &s_track_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( s_track_unordered_map2_Turbo, s_track_unordered_map2 );
        }
      }
      break;
    case LHCb::CLID_RecVertex:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (s_recvertex_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (s_recvertex_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &s_recvertex_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( s_recvertex_unordered_map2_Turbo, s_recvertex_unordered_map2) ;
        }
      }
      break;
    case LHCb::CLID_Vertex:
      used_map = pick_map( s_vertex_unordered_map2_Turbo, s_vertex_unordered_map2);
      break;
    case LHCb::CLID_Particle:
      {
        if(m_version<3) {
          // Looking at Run 1 data, need to know which map to use
          if( subbank->size() == (s_particle_unordered_map2.at(1)).size() ) run1version = 1;
          else if( subbank->size() == (s_particle_unordered_map2.at(2)).size() ) run1version = 2;
          used_map = &s_particle_unordered_map2;
          m_version=run1version;
        } else {
          used_map = pick_map( s_particle_unordered_map2_Turbo, s_particle_unordered_map2 );
        }
      }
      break;
    case LHCb::CLID_ProtoParticle:
      used_map = pick_map(s_proto_unordered_map2_Turbo, s_proto_unordered_map2);
      break;
    case LHCb::CLID_RichPID:
      used_map = pick_map(s_rpid_unordered_map2_Turbo, s_rpid_unordered_map2);
      break;
    case LHCb::CLID_CaloHypo:
      used_map = pick_map(s_calohypo_unordered_map2_Turbo, s_calohypo_unordered_map2);
      break;
    case LHCb::CLID_MuonPID:
      used_map = pick_map(s_mpid_unordered_map2_Turbo,  s_mpid_unordered_map2);
      break;
    case LHCb::CLID_RecSummary:
      {
        used_map = &s_recsummary_unordered_map2;
      }
      break;
    case 40: // This is a special number to deal with the holder of related info, contains only the location enum
      {
        info->insert( "0#LocationID", pun_to<float>( (*subbank)[ 0 ]));
        return;
      }
      break;
    case 41: // This is a special number to deal with the actual related info
      {
        unsigned int n=0;
        do {
          // First make map key
          std::stringstream key;
          int len = snprintf(nullptr, 0, "%d", n);
          int padding = 4 - len;
          for(int m=0; m<padding; m++) key << "0";
          key << n << "#GenericKey";
          info->insert(key.str().c_str(),pun_to<float>( (*subbank)[ n ]));
          n++;

          // Then do the same for the value
          std::stringstream keykey;
          int len2 = snprintf(nullptr, 0, "%d", n);
          int padding2 = 4 - len2;
          for(int m=0; m<padding2; m++) keykey << "0";
          keykey << n << "#GenericValue";
          info->insert(keykey.str().c_str(),pun_to<float>( (*subbank)[ n ]));
          n++;
        }while(n<subbank->size());
        return;
      }
      break;
    case LHCb::CLID_CaloCluster:
      if(m_version<3) {
        // Looking at Run 1 data, need to know which map to use
        if( subbank->size() == (s_calo_unordered_map2.at(1)).size() ) run1version = 1;
        else if( subbank->size() == (s_calo_unordered_map2.at(2)).size() ) run1version = 2;
        used_map = &s_calo_unordered_map2;
        m_version=run1version;
      } else {
        used_map = pick_map(s_calo_unordered_map2_Turbo, s_calo_unordered_map2);
      }
      break;
      // NOTE THE CASE OF 1 IS TAKEN CARE OF INSIDE THE DECODER
    default:
      {

        Warning( std::string{ " StdInfo on unsupported class type "}+ std::to_string(classID),
                  StatusCode::SUCCESS, 20 ).ignore();
        int e = 0;
        for (const auto& i : (*subbank)) {
           info->insert( std::string { "z#Unknown.unknown" } + std::to_string( e++ ),
                                  pun_to<float>(i) );
        }
        return;
      }
    }

  // If version<3 => for Track, Particle, RecVertex, and CaloCluster, need to assign version based on size

  if(UNLIKELY(msgLevel(MSG::DEBUG)))
    debug() << "Inserting from bank" << endmsg;
  auto version_to_use = findBestPrevious( *used_map, m_version );
  const auto& table_to_use = used_map->at( version_to_use );
  for(const auto& object : table_to_use) {
    if( object.second.first >= static_cast<int>(subbank->size()) ) {
      Error(std::string{"Requested out-of-range index "}
          + std::to_string(object.second.first) + " in subbank of size "
          + std::to_string(subbank->size())).ignore();
    }
    auto float_value = pun_to<float>( (*subbank)[ object.second.first ] );
    info->insert( object.first, float_value );
  }
}

// Put the information in to the HltObjectSummary
void ReportConvertTool::ParticleObject2Summary( HltObjectSummary::Info* info, const LHCb::Particle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map
    = ( turbo ? s_particle_unordered_map2_Turbo : s_particle_unordered_map2 );

  for(const auto&  particle: used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( particle.second.second )
    {
      case 0: info->insert( particle.first, float( object->particleID().pid() ) ); break;
      case 1: info->insert( particle.first, float( object->measuredMass() ) ); break;
      case 2: info->insert( particle.first, float( object->referencePoint().z() ) ); break;
      case 3: info->insert( particle.first, float( object->referencePoint().x() ) ); break;
      case 4: info->insert( particle.first, float( object->referencePoint().y() ) ); break;
      case 5: info->insert( particle.first, float( object->slopes().x() ) ); break;
      case 6: info->insert( particle.first, float( object->slopes().y() ) ); break;
      case 7: info->insert( particle.first, float( 1.0/(object->p()) ) ); break;
      case 8: info->insert( particle.first, float( object->confLevel() ) ); break;
      case 9: info->insert( particle.first, float( object->measuredMassErr() ) ); break;
      case 10: info->insert( particle.first, float( object->momCovMatrix()(0,0) ) ); break;
      case 11: info->insert( particle.first, float( object->momCovMatrix()(1,1) ) ); break;
      case 12: info->insert( particle.first, float( object->momCovMatrix()(2,2) ) ); break;
      case 13: info->insert( particle.first, float( object->momCovMatrix()(3,3) ) ); break;
      case 14: info->insert( particle.first, float( object->momCovMatrix()(1,0) ) ); break;
      case 15: info->insert( particle.first, float( object->momCovMatrix()(2,0) ) ); break;
      case 16: info->insert( particle.first, float( object->momCovMatrix()(2,1) ) ); break;
      case 17: info->insert( particle.first, float( object->momCovMatrix()(3,0) ) ); break;
      case 18: info->insert( particle.first, float( object->momCovMatrix()(3,1) ) ); break;
      case 19: info->insert( particle.first, float( object->momCovMatrix()(3,2) ) ); break;
      case 20: info->insert( particle.first, float( object->posMomCovMatrix()(0,0) ) ); break;
      case 21: info->insert( particle.first, float( object->posMomCovMatrix()(1,1) ) ); break;
      case 22: info->insert( particle.first, float( object->posMomCovMatrix()(2,2) ) ); break;
      case 23: info->insert( particle.first, float( object->posMomCovMatrix()(1,0) ) ); break;
      case 24: info->insert( particle.first, float( object->posMomCovMatrix()(0,1) ) ); break;
      case 25: info->insert( particle.first, float( object->posMomCovMatrix()(2,0) ) ); break;
      case 26: info->insert( particle.first, float( object->posMomCovMatrix()(0,2) ) ); break;
      case 27: info->insert( particle.first, float( object->posMomCovMatrix()(2,1) ) ); break;
      case 28: info->insert( particle.first, float( object->posMomCovMatrix()(1,2) ) ); break;
      case 29: info->insert( particle.first, float( object->posMomCovMatrix()(3,0) ) ); break;
      case 30: info->insert( particle.first, float( object->posMomCovMatrix()(3,1) ) ); break;
      case 31: info->insert( particle.first, float( object->posMomCovMatrix()(3,2) ) ); break;
      case 32: info->insert( particle.first, float( object->posCovMatrix()(0,0) ) ); break;
      case 33: info->insert( particle.first, float( object->posCovMatrix()(1,1) ) ); break;
      case 34: info->insert( particle.first, float( object->posCovMatrix()(2,2) ) ); break;
      case 35: info->insert( particle.first, float( object->posCovMatrix()(1,0) ) ); break;
      case 36: info->insert( particle.first, float( object->posCovMatrix()(2,0) ) ); break;
      case 37: info->insert( particle.first, float( object->posCovMatrix()(2,1) ) ); break;
      case 38: info->insert( particle.first, float( object->momentum().M() ) ); break;
      case 39: info->insert( particle.first, float( object->momentum().px() ) ); break;
      case 40: info->insert( particle.first, float( object->momentum().py() ) ); break;
      case 41: info->insert( particle.first, float( object->momentum().pz() ) ); break;
    }
  }

}

void ReportConvertTool::ProtoParticleObject2Summary( HltObjectSummary::Info* info, const LHCb::ProtoParticle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? s_proto_unordered_map2_Turbo : s_proto_unordered_map2 );

  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    assert(proto.second.second < static_cast<int>(s_proto_map.size()));
    info->insert( proto.first, float( object->info( s_proto_map[proto.second.second], -1000 ) ) );
  }

}

void ReportConvertTool::TrackObject2Summary( HltObjectSummary::Info* info, const LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_track_unordered_map2_Turbo
                                : s_track_unordered_map2 );

  LHCb::State first, last;
  if( object->type() == LHCb::Track::Types::Long ){
    if( object->hasStateAt(LHCb::State::Location::ClosestToBeam) ) first = *(object->stateAt(LHCb::State::Location::ClosestToBeam));
    else first = *(object->states().front());
    //
    if( object->hasStateAt(LHCb::State::Location::BegRich2) ) last = *(object->stateAt(LHCb::State::Location::BegRich2));
    else last = *(object->states().back());
  }
  else if( object->type() == LHCb::Track::Types::Downstream ){
    if( object->hasStateAt(LHCb::State::Location::FirstMeasurement) ) first = *(object->stateAt(LHCb::State::Location::FirstMeasurement));
    else first = *(object->states().front());
    //
    if( object->hasStateAt(LHCb::State::Location::BegRich2) ) last = *(object->stateAt(LHCb::State::Location::BegRich2));
    else last = *(object->states().back());
  }
  else{
    first = *(object->states().front());
    last = *(object->states().back());
  }

  for(const auto& track : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( track.second.second )
    {
      case 0: info->insert( track.first, float( first.z() ) ); break;
      case 1: info->insert( track.first, float( first.x() ) ); break;
      case 2: info->insert( track.first, float( first.y() ) ); break;
      case 3: info->insert( track.first, float( first.tx() ) ); break;
      case 4: info->insert( track.first, float( first.ty() ) ); break;
      case 5: info->insert( track.first, float( first.qOverP() ) ); break;
      case 6: info->insert( track.first, float( object->chi2PerDoF() ) ); break;
      case 7: info->insert( track.first, float( object->nDoF() ) ); break;
      case 8: info->insert( track.first, float( object->likelihood() ) ); break;
      case 9: info->insert( track.first, float( object->ghostProbability() ) ); break;
      case 10: info->insert( track.first, float( object->flags() ) ); break;
      case 11: info->insert( track.first, float( last.z() ) ); break;
      case 12: info->insert( track.first, float( last.x() ) ); break;
      case 13: info->insert( track.first, float( last.y() ) ); break;
      case 14: info->insert( track.first, float( last.tx() ) ); break;
      case 15: info->insert( track.first, float( last.ty() ) ); break;
      case 16: info->insert( track.first, float( last.qOverP() ) ); break;
      case 17: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::CloneDist, -1000) ) ); break;
      case 18: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::FitMatchChi2, -1000) ) ); break;
      case 19: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::FitVeloChi2, -1000) ) ); break;
      case 20: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::FitTChi2, -1000) ) ); break;
      case 21: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::FitVeloNDoF, -1000) ) ); break;
      case 22: info->insert( track.first, float( object->info( LHCb::Track::AdditionalInfo::FitTNDoF, -1000) ) ); break;
      case 23: info->insert( track.first, float( first.flags() ) ); break;
      case 24: info->insert( track.first, float( last.flags() ) ); break;
      case 25: info->insert( track.first, float( first.covariance()(0,0) ) ); break;
      case 26: info->insert( track.first, float( first.covariance()(1,1) ) ); break;
      case 27: info->insert( track.first, float( first.covariance()(2,2) ) ); break;
      case 28: info->insert( track.first, float( first.covariance()(3,3) ) ); break;
      case 29: info->insert( track.first, float( first.covariance()(4,4) ) ); break;
      case 30: info->insert( track.first, float( first.covariance()(0,1) ) ); break;
      case 31: info->insert( track.first, float( first.covariance()(0,2) ) ); break;
      case 32: info->insert( track.first, float( first.covariance()(0,3) ) ); break;
      case 33: info->insert( track.first, float( first.covariance()(0,4) ) ); break;
      case 34: info->insert( track.first, float( first.covariance()(1,2) ) ); break;
      case 35: info->insert( track.first, float( first.covariance()(1,3) ) ); break;
      case 36: info->insert( track.first, float( first.covariance()(1,4) ) ); break;
      case 37: info->insert( track.first, float( first.covariance()(2,3) ) ); break;
      case 38: info->insert( track.first, float( first.covariance()(2,4) ) ); break;
      case 39: info->insert( track.first, float( first.covariance()(3,4) ) ); break;
    }
  }
}

void ReportConvertTool::RichPIDObject2Summary( HltObjectSummary::Info* info, const LHCb::RichPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? s_rpid_unordered_map2_Turbo
                                 : s_rpid_unordered_map2 );

  for(const auto& rpid : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    switch( rpid.second.second )
    {
      case 0: info->insert( rpid.first, float( object->pidResultCode() ) ); break;
      case 1: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Electron ) ) ); break;
      case 2: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Muon ) ) ); break;
      case 3: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Pion ) ) ); break;
      case 4: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Kaon ) ) ); break;
      case 5: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Proton ) ) ); break;
      case 6: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::BelowThreshold ) ) ); break;
      case 7: info->insert( rpid.first, float( object->particleDeltaLL( Rich::ParticleIDType::Deuteron ) ) ); break;
    }
  }

}

void ReportConvertTool::MuonPIDObject2Summary( HltObjectSummary::Info* info , const LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_mpid_unordered_map2_Turbo
                                : s_mpid_unordered_map2 );
  for(const auto& mpid : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( mpid.second.second )
    {
      case 0: info->insert( mpid.first, float( object->MuonLLMu() ) ); break;
      case 1: info->insert( mpid.first, float( object->MuonLLBg() ) ); break;
      case 2: info->insert( mpid.first, float( object->nShared() ) ); break;
      case 3: info->insert( mpid.first, float( object->Status() ) ); break;
      case 4: info->insert( mpid.first, float( object->IsMuon() ) ); break;
      case 5: info->insert( mpid.first, float( object->IsMuonLoose() ) ); break;
      case 6: info->insert( mpid.first, float( object->IsMuonTight() ) ); break;
      case 7: info->insert( mpid.first, float( object->chi2Corr() ) ); break;
      case 8: info->insert( mpid.first, float( object->muonMVA1() ) ); break;
      case 9: info->insert( mpid.first, float( object->muonMVA2() ) ); break;
      case 10: info->insert( mpid.first, float( object->muonMVA3() ) ); break;
      case 11: info->insert( mpid.first, float( object->muonMVA4() ) ); break;
    }
  }

}

void ReportConvertTool::CaloClusterObject2Summary( HltObjectSummary::Info* info, const LHCb::CaloCluster* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_calo_unordered_map2_Turbo
                                : s_calo_unordered_map2 );
  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: info->insert( calo.first, float( object->e() ) ); break;
      case 1: info->insert( calo.first, float( object->position().x() ) ); break;
      case 2: info->insert( calo.first, float( object->position().y() ) ); break;
      case 3: info->insert( calo.first, float( object->position().z() ) ); break;
    }
  }
}

void ReportConvertTool::CaloHypoObject2Summary( HltObjectSummary::Info* info, const LHCb::CaloHypo* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_calohypo_unordered_map2_Turbo
                                : s_calohypo_unordered_map2 );
  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: info->insert( calo.first, float( object->e() ) ); break;
      case 1: info->insert( calo.first, float( (object->position() ? object->position()->x() : -1.0) ) ); break;
      case 2: info->insert( calo.first, float( (object->position() ? object->position()->y() : -1.0) ) ); break;
      case 3: info->insert( calo.first, float( (object->position() ? object->position()->z() : -1.0) ) ); break;
      case 4: info->insert( calo.first, float( object->lh() ) ); break;
      case 5: info->insert( calo.first, float( object->hypothesis() ) ); break;
    }
  }
}

void ReportConvertTool::RecVertexObject2Summary( HltObjectSummary::Info* info, const LHCb::RecVertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_recvertex_unordered_map2_Turbo
                                : s_recvertex_unordered_map2 );
  for(const auto&  recvertex : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( recvertex.second.second )
    {
      case 0: info->insert( recvertex.first, float( object->position().x() ) ); break;
      case 1: info->insert( recvertex.first, float( object->position().y() ) ); break;
      case 2: info->insert( recvertex.first, float( object->position().z() ) ); break;
      case 3: info->insert( recvertex.first, float( object->chi2() ) ); break;
      case 4: info->insert( recvertex.first, float( object->nDoF() ) ); break;
      case 5: info->insert( recvertex.first, float( object->technique() ) ); break;
      case 6: info->insert( recvertex.first, float( object->covMatrix()(0,0) ) ); break;
      case 7: info->insert( recvertex.first, float( object->covMatrix()(1,1) ) ); break;
      case 8: info->insert( recvertex.first, float( object->covMatrix()(2,2) ) ); break;
      case 9: info->insert( recvertex.first, float( object->covMatrix()(1,0) ) ); break;
      case 10: info->insert( recvertex.first, float( object->covMatrix()(2,0) ) ); break;
      case 11: info->insert( recvertex.first, float( object->covMatrix()(2,1) ) ); break;
    }
  }

}

void ReportConvertTool::VertexObject2Summary( HltObjectSummary::Info* info, const LHCb::Vertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_vertex_unordered_map2_Turbo
                                : s_vertex_unordered_map2 );
  for(const auto&  vertex : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( vertex.second.second )
    {
      case 0: info->insert( vertex.first, float( object->chi2() ) ); break;
      case 1: info->insert( vertex.first, float( object->nDoF() ) ); break;
      case 2: info->insert( vertex.first, float( object->position().x() ) ); break;
      case 3: info->insert( vertex.first, float( object->position().y() ) ); break;
      case 4: info->insert( vertex.first, float( object->position().z() ) ); break;
      case 5: info->insert( vertex.first, float( object->technique() ) ); break;
      case 6: info->insert( vertex.first, float( object->covMatrix()(0,0) ) ); break;
      case 7: info->insert( vertex.first, float( object->covMatrix()(1,1) ) ); break;
      case 8: info->insert( vertex.first, float( object->covMatrix()(2,2) ) ); break;
      case 9: info->insert( vertex.first, float( object->covMatrix()(1,0) ) ); break;
      case 10: info->insert( vertex.first, float( object->covMatrix()(2,0) ) ); break;
      case 11: info->insert( vertex.first, float( object->covMatrix()(2,1) ) ); break;
    }
  }

}

void ReportConvertTool::RecSummaryObject2Summary( HltObjectSummary::Info* info, const LHCb::RecSummary* object ) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = s_recsummary_unordered_map2;
  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) )) {
    assert( recsummary.second.second < static_cast<int>(s_rsum_map.size()));
    info->insert( recsummary.first, float( object->info( s_rsum_map[recsummary.second.second],0 ) ) );
  }

}

// Convert generic Gaudi::VectorMap into a summary
void ReportConvertTool::GenericMapObject2Summary( HltObjectSummary::Info* info , const GaudiUtils::VectorMap<short,float>* map) {
  int n=0;
  for( auto i : *map ){
    // First put the map key inside
    std::stringstream key;
    int len = snprintf(nullptr, 0, "%d", n);
    int padding = 4 - len;
    for(int m=0; m<padding; m++) key << "0";
    key << n << "#GenericKey";
    n++;
    info->insert( key.str().c_str() , float( i.first ) );

    // Then do the same for the value
    std::stringstream keykey;
    int len2 = snprintf(nullptr, 0, "%d", n);
    int padding2 = 4 - len2;
    for(int m=0; m<padding2; m++) keykey << "0";
    keykey << n << "#GenericValue";
    n++;
    info->insert( keykey.str().c_str() , float( i.second ) );
  }
}

// Put the information in the summary back in the object
void ReportConvertTool::ParticleObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Particle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_particle_unordered_map2_Turbo
                                : s_particle_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    auto Isize = count_info(*info);
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (s_particle_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (s_particle_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Particle requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 ).ignore();
    m_version=run1version;
  }

  // Raw
  double raw_m {0.0}, raw_p1 {0.0}, raw_p2 {0.0}, raw_p3 {0.0};

  // Momentum components
  float p=0;
  float slopex=0;
  float slopey=0;
  // Reference point
  Gaudi::XYZPoint xyz;
  // Matrices
  Gaudi::SymMatrix4x4 & momCov = const_cast<Gaudi::SymMatrix4x4&>(object->momCovMatrix());
  Gaudi::Matrix4x3 & posMomCov = const_cast<Gaudi::Matrix4x3&>(object->posMomCovMatrix());
  Gaudi::SymMatrix3x3 & posCov = const_cast<Gaudi::SymMatrix3x3&>(object->posCovMatrix());

  for(const auto& particle : used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x = (*info)[particle.first];
    switch( particle.second.second )
    {
      case 0: object->setParticleID( LHCb::ParticleID( x ) ); break;
      case 1: object->setMeasuredMass( x ); break;
      case 2: xyz.SetZ( x ); break;
      case 3: xyz.SetX( x );  break;
      case 4: xyz.SetY( x ); break;
      case 5: slopex = x ; break;
      case 6: slopey = x ; break;
      case 7: p=1.0/( x ); break;
      case 8: object->setConfLevel( x ); break;
      case 9: object->setMeasuredMassErr( x ); break;
      case 10: momCov(0,0) = x ; break;
      case 11: momCov(1,1) = x ; break;
      case 12: momCov(2,2) = x ; break;
      case 13: momCov(3,3) = x ; break;
      case 14: momCov(1,0) = x ; break;
      case 15: momCov(2,0) = x ; break;
      case 16: momCov(2,1) = x ; break;
      case 17: momCov(3,0) = x ; break;
      case 18: momCov(3,1) = x ; break;
      case 19: momCov(3,2) = x ; break;
      case 20: posMomCov(0,0) = x ; break;
      case 21: posMomCov(1,1) = x ; break;
      case 22: posMomCov(2,2) = x ; break;
      case 23: posMomCov(1,0) = x ; break;
      case 24: posMomCov(0,1) = x ; break;
      case 25: posMomCov(2,0) = x ; break;
      case 26: posMomCov(0,2) = x ; break;
      case 27: posMomCov(2,1) = x ; break;
      case 28: posMomCov(1,2) = x ; break;
      case 29: posMomCov(3,0) = x ; break;
      case 30: posMomCov(3,1) = x ; break;
      case 31: posMomCov(3,2) = x ; break;
      case 32: posCov(0,0) = x ; break;
      case 33: posCov(1,1) = x ; break;
      case 34: posCov(2,2) = x ; break;
      case 35: posCov(1,0) = x ; break;
      case 36: posCov(2,0) = x ; break;
      case 37: posCov(2,1) = x ; break;
      case 38: raw_m = x ; break;
      case 39: raw_p1 = x ; break;
      case 40: raw_p2 = x ; break;
      case 41: raw_p3 = x ; break;
    }
  }
  object->setReferencePoint(xyz);

  // Old way with slopes
  const double slopez = 1/sqrt(slopex*slopex + slopey*slopey + 1.0);
  const double pz = slopez*p;
  const double px = slopex*pz;
  const double py = slopey*pz;
  const double mm = object->measuredMass();
  const double pe = sqrt(p*p+mm*mm);
  Gaudi::LorentzVector part_mom(px,py,pz,pe);
  //

  double psq = raw_p1*raw_p1 + raw_p2*raw_p2 + raw_p3*raw_p3;
  double raw_pe = sqrt(psq+raw_m*raw_m);
  Gaudi::LorentzVector part_mom_raw(raw_p1,raw_p2,raw_p3,raw_pe);
  if(raw_p3>0.001) object->setMomentum(part_mom_raw);
  else object->setMomentum(part_mom);
}

void ReportConvertTool::ProtoParticleObjectFromSummary( const HltObjectSummary::Info* info, LHCb::ProtoParticle* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_proto_unordered_map2_Turbo
                                : s_proto_unordered_map2 );
  for(const auto& proto : used_map.at( findBestPrevious( used_map, m_version ) )) {
    assert(proto.second.second<static_cast<int>(s_proto_map.size()));
    auto i = (*info)[proto.first];
    if (i != -1000 ) object->addInfo( s_proto_map[proto.second.second], i);
  }

}

void ReportConvertTool::TrackObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Track* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_track_unordered_map2_Turbo
                                : s_track_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    auto Isize = count_info( *info );
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (s_track_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (s_track_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 ).ignore();
    m_version=run1version;
  }

  LHCb::State first;
  LHCb::State  last;

  Gaudi::TrackSymMatrix cov;
  for(const auto& track : used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x = (*info)[track.first];
    switch( track.second.second )
    {
      case 0: first.setZ( x ); break;
      case 1: first.setX( x ); break;
      case 2: first.setY( x ); break;
      case 3: first.setTx( x ); break;
      case 4: first.setTy( x ); break;
      case 5: first.setQOverP( x ); break;
      case 6: object->setChi2PerDoF( x ); break;
      case 7: object->setNDoF( int( x ) ); break;
      case 8: object->setLikelihood( x ); break;
      case 9: object->setGhostProbability( x ); break;
      case 10: object->setFlags( (unsigned int)x ); break;
      case 11: last.setZ( x ); break;
      case 12: last.setX( x ); break;
      case 13: last.setY( x ); break;
      case 14: last.setTx( x ); break;
      case 15: last.setTy( x ); break;
      case 16: last.setQOverP( x ); break;
      case 17: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::CloneDist, x ); break;
      case 18: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::FitMatchChi2, x ); break;
      case 19: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::FitVeloChi2, x ); break;
      case 20: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::FitTChi2, x ); break;
      case 21: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::FitVeloNDoF, x ); break;
      case 22: if( x != -1000 ) object->addInfo( LHCb::Track::AdditionalInfo::FitTNDoF, x ); break;
      case 23: first.setFlags( x ); break;
      case 24: last.setFlags( x ); break;
      case 25: cov(0,0) = x ; break;
      case 26: cov(1,1) = x ; break;
      case 27: cov(2,2) = x ; break;
      case 28: cov(3,3) = x ; break;
      case 29: cov(4,4) = x ; break;
      case 30: cov(0,1) = x ; break;
      case 31: cov(0,2) = x ; break;
      case 32: cov(0,3) = x ; break;
      case 33: cov(0,4) = x ; break;
      case 34: cov(1,2) = x ; break;
      case 35: cov(1,3) = x ; break;
      case 36: cov(1,4) = x ; break;
      case 37: cov(2,3) = x ; break;
      case 38: cov(2,4) = x ; break;
      case 39: cov(3,4) = x ; break;
    }
  }

  // I am sure there is a smarter way of doing this
  if(cov(0,0)>0.0){
    cov(1,0)=cov(0,1);
    cov(2,0)=cov(0,2);
    cov(3,0)=cov(0,3);
    cov(4,0)=cov(0,4);
    cov(2,1)=cov(1,2);
    cov(3,1)=cov(1,3);
    cov(4,1)=cov(1,4);
    cov(3,2)=cov(2,3);
    cov(4,2)=cov(2,4);
    cov(4,3)=cov(3,4);
  }
  first.setCovariance(cov);

  object->addToStates(std::move(first));
  if(turbo) object->addToStates(std::move(last));
}

void ReportConvertTool::RichPIDObjectFromSummary( const HltObjectSummary::Info*info, LHCb::RichPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = ( turbo ? s_rpid_unordered_map2_Turbo
                                 : s_rpid_unordered_map2 );

  for(const auto& rpid : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    auto x = (*info)[rpid.first];
    switch( rpid.second.second )
    {
      case 0: object->setPidResultCode( (unsigned int)x ); break;
      case 1: object->setParticleDeltaLL( Rich::ParticleIDType::Electron, x ); break;
      case 2: object->setParticleDeltaLL( Rich::ParticleIDType::Muon, x ); break;
      case 3: object->setParticleDeltaLL( Rich::ParticleIDType::Pion, x ); break;
      case 4: object->setParticleDeltaLL( Rich::ParticleIDType::Kaon, x ); break;
      case 5: object->setParticleDeltaLL( Rich::ParticleIDType::Proton, x ); break;
      case 6: object->setParticleDeltaLL( Rich::ParticleIDType::BelowThreshold, x ); break;
      case 7: object->setParticleDeltaLL( Rich::ParticleIDType::Deuteron, x ); break;
    }
  }

}

void ReportConvertTool::MuonPIDObjectFromSummary( const HltObjectSummary::Info* info, LHCb::MuonPID* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_mpid_unordered_map2_Turbo
                                : s_mpid_unordered_map2 );

  for(const auto& mpid: used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x = (*info)[mpid.first];
    switch( mpid.second.second )
    {
      case 0: object->setMuonLLMu( x ); break;
      case 1: object->setMuonLLBg( x ); break;
      case 2: object->setNShared( int( x ) ); break;
      case 3: object->setStatus( int( x ) ); break;
      case 4: object->setIsMuon( int( x ) ); break;
      case 5: object->setIsMuonLoose( int( x ) ); break;
      case 6: object->setIsMuonTight( int( x ) ); break;
      case 7: object->setChi2Corr( x ); break;
      case 8: object->setMuonMVA1( x ); break;
      case 9: object->setMuonMVA2( x ); break;
      case 10: object->setMuonMVA3( x ); break;
      case 11: object->setMuonMVA4( x ); break;
    }
  }

}

void ReportConvertTool::CaloClusterObjectFromSummary( const HltObjectSummary::Info* info, LHCb::CaloCluster* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_calo_unordered_map2_Turbo
                                : s_calo_unordered_map2 );
  Gaudi::Vector3 & xye = *(const_cast<Gaudi::Vector3*>(&object->position().parameters()));
  //
  xye(0)=0;
  xye(1)=0;
  xye(2)=0;

  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x = (*info)[calo.first];
    switch( calo.second.second )
    {
      case 0:  // [[fallthrough]]
      case 1:  // [[fallthrough]]
      case 2:
        xye(calo.second.second) = x; break;
      case 3:
        object->position().setZ( x ); break;
    }
  }
}

void ReportConvertTool::CaloHypoObjectFromSummary( const HltObjectSummary::Info* info, LHCb::CaloHypo* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_calohypo_unordered_map2_Turbo
                                : s_calohypo_unordered_map2 );
  object->setPosition(std::make_unique<LHCb::CaloHypo::Position>());

  Gaudi::Vector3 & xye = *(const_cast<Gaudi::Vector3*>(&object->position()->parameters()));
  //
  double e=0;
  double x=0;
  double y=0;
  double z=0;

  for(const auto& calo : used_map.at( findBestPrevious( used_map, m_version ) )) {
    switch( calo.second.second )
    {
      case 0: e = (*info)[ calo.first ]; break;
      case 1: x = (*info)[ calo.first ]; break;
      case 2: y = (*info)[ calo.first ]; break;
      case 3: z = (*info)[ calo.first ]; break;
      case 4: object->setLh( (*info)[ calo.first ] ); break;
      case 5: object->setHypothesis( static_cast<LHCb::CaloHypo::Hypothesis>( (*info)[ calo.first ] ) ); break;
    }
  }
  if(fabs(z+1.0)>0.01){
    xye(0) = x;
    xye(1) = y;
    xye(2) = e;
    object->position()->setZ( z );
  }
}

void ReportConvertTool::RecVertexObjectFromSummary( const HltObjectSummary::Info* info, LHCb::RecVertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_recvertex_unordered_map2_Turbo
                                : s_recvertex_unordered_map2 );
  if( m_version < 3 ){
    int run1version=-999;
    // find size we care about (i.e. make sure extra info not counted)
    unsigned int Isize = count_info( *info );
    // Looking at Run 1 data, need to know which map to use
    if( Isize == (s_recvertex_unordered_map2.at(1)).size() ) run1version = 1;
    else if( Isize == (s_recvertex_unordered_map2.at(2)).size() ) run1version = 2;
    else Error( "Track requested in Run 1 reports, but reports are unknown" , StatusCode::FAILURE, 100 ).ignore();
    m_version=run1version;
  }

  Gaudi::XYZPoint xyz;
  Gaudi::SymMatrix3x3 & cov = *(const_cast<Gaudi::SymMatrix3x3*>(&object->covMatrix()));
  for(const auto& recvertex: used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x = (*info)[recvertex.first];
    switch( recvertex.second.second )
    {
      case 0: xyz.SetX( x ); break;
      case 1: xyz.SetY( x ); break;
      case 2: xyz.SetZ( x ); break;
      case 3: object->setChi2( x ); break;
      case 4: object->setNDoF( int( x ) ); break;
      case 5: object->setTechnique( static_cast<LHCb::RecVertex::RecVertexType>( x ) ); break;
      case 6: cov(0,0) = x ; break;
      case 7: cov(1,1) = x ; break;
      case 8: cov(2,2) = x ; break;
      case 9: cov(1,0) = x ; break;
      case 10: cov(2,0) = x ; break;
      case 11: cov(2,1) = x ; break;
    }
  }
  object->setPosition( xyz );

}

void ReportConvertTool::VertexObjectFromSummary( const HltObjectSummary::Info* info, LHCb::Vertex* object, bool turbo) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = (turbo ? s_vertex_unordered_map2_Turbo
                                : s_vertex_unordered_map2 );
  Gaudi::XYZPoint xyz;
  Gaudi::SymMatrix3x3 & cov = *(const_cast<Gaudi::SymMatrix3x3*>(&object->covMatrix()));

  for(const auto& vertex: used_map.at( findBestPrevious( used_map, m_version ) )) {
    auto x= (*info)[vertex.first];
    switch( vertex.second.second )
    {
      case 0: object->setChi2( x ); break;
      case 1: object->setNDoF( int( x ) ); break;
      case 2: xyz.SetX( x ); break;
      case 3: xyz.SetY( x ); break;
      case 4: xyz.SetZ( x ); break;
      case 5: object->setTechnique( static_cast<LHCb::Vertex::CreationMethod>( x ) ); break;
      case 6: cov(0,0) = x ; break;
      case 7: cov(1,1) = x ; break;
      case 8: cov(2,2) = x ; break;
      case 9: cov(1,0) = x ; break;
      case 10: cov(2,0) = x ; break;
      case 11: cov(2,1) = x ; break;
    }
  }
  object->setPosition( xyz );
}

void ReportConvertTool::RecSummaryObjectFromSummary( const HltObjectSummary::Info* info, LHCb::RecSummary* object) {
  if( m_version == -999 ){
    Warning( "I have not been told a version number to use, assuming the latest", StatusCode::SUCCESS, 20 ).ignore();
    m_version = m_LatestVersion;
  }

  const auto& used_map = s_recsummary_unordered_map2;
  for(const auto& recsummary : used_map.at( findBestPrevious( used_map, m_version ) ) ) {
    assert(recsummary.second.second<static_cast<int>(s_rsum_map.size()));
    object->addInfo( s_rsum_map[recsummary.second.second], (*info)[ recsummary.first ] );
  }
}

// Convert generic Gaudi::VectorMap summary back into the object
void ReportConvertTool::GenericMapObjectFromSummary( const HltObjectSummary::Info* info , GaudiUtils::VectorMap<short,float>* map) {
  unsigned int n=0;
  while(n < (info->size()) ){
    // First put the map key inside
    std::stringstream key;
    int len = snprintf(nullptr, 0, "%d", n);
    int padding = 4 - len;
    for(int m=0; m<padding; m++) key << "0";
    key << n << "#GenericKey";
    n++;

    short i_key = short( (*info)[key.str().c_str()] );
    // Then do the same for the value
    std::stringstream keykey;
    int len2 = snprintf(nullptr, 0, "%d", n);
    int padding2 = 4 - len2;
    for(int m=0; m<padding2; m++) keykey << "0";
    keykey << n << "#GenericValue";
    n++;

    float i_value = float( (*info)[keykey.str().c_str()] );

    map->insert(std::pair<short,float>(i_key,i_value));
  }
}
