
// local
#include "Event/PackedCaloAdc.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

void CaloAdcPacker::pack( const DataVector & adcs, PackedDataVector & padcs ) const
{
  if ( isSupportedVer(padcs.packingVersion()) )
  {
    padcs.data().reserve( adcs.size() );
    for ( const auto * adc : adcs ){
      // make a new packed object
      padcs.data().emplace_back( PackedData() );
      auto & padc = padcs.data().back();
      // general
      padc.key  = (adc->key()).all();
      padc.adc  = adc->adc() ;
    }
  }
}

void CaloAdcPacker::unpack( const PackedDataVector & padcs,
                            DataVector             & adcs ) const
{
  if ( isSupportedVer(padcs.packingVersion()) )
  {
    adcs.reserve( padcs.data().size() );
    for ( const auto & padc : padcs.data() )
    {
      // make and save new adc container, with original key
      auto * adc  = new Data();
      adcs.insert( adc, LHCb::CaloCellID(padc.key) );
      // general
      adc->setAdc( padc.adc );
    }
  }
}

StatusCode CaloAdcPacker::check( const DataVector & dataA,
                                 const DataVector & dataB ) const
{
  StatusCode sc = StatusCode::SUCCESS;

  // Loop over data containers together and compare
  auto iA(dataA.begin()), iB(dataB.begin());
  for ( ; iA != dataA.end() && iB != dataB.end(); ++iA, ++iB )
  {
    if ( sc ) sc = check( **iA, **iB );
  }
  // Return final status
  return sc;
}

StatusCode CaloAdcPacker::check( const Data & dataA,
                                 const Data & dataB ) const{

  // assume OK from the start
  bool ok = true;

  // checker
  const DataPacking::DataChecks ch(parent());

  // checks here

  // key
  ok &= ch.compareInts( "key", dataA.key(), dataB.key() );
  // energy
  ok &= ch.compareDoubles( "type", dataA.adc(), dataB.adc() );


  // force printout for tests
  //ok = false;
  // If comparison not OK, print full information
  if ( !ok ){ const std::string loc = ( dataA.parent() && dataA.parent()->registry() ?
                              dataA.parent()->registry()->identifier() : "Not in TES" );
    parent().warning() << "Problem with CaloAdc data packing :-" << endmsg
                       << "  Original Adc key=" << dataA.key()
                       << " in '" << loc << "'" << endmsg
                       << dataA << endmsg
                       << "  Unpacked Adc" << endmsg
                       << dataB << endmsg;
  }
  return ( ok ? StatusCode::SUCCESS : StatusCode::FAILURE );
}
