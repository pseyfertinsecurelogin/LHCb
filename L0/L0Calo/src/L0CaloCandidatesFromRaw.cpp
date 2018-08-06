// Include files
// local
#include "L0CaloCandidatesFromRaw.h"

// From Event
#include "Event/RawBankReadoutStatus.h"
#include "Event/L0CaloCandidate.h"
#include "Event/L0ProcessorData.h"

// Local
#include "L0Candidate.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0CaloCandidatesFromRaw
//
// 2003-12-15 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( L0CaloCandidatesFromRaw )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
L0CaloCandidatesFromRaw::L0CaloCandidatesFromRaw( const std::string& name,
                                                  ISvcLocator* pSvcLocator)
  : L0FromRawBase ( name , pSvcLocator )
  , m_convertTool(NULL)
{

}

//=============================================================================
// Destructor
//=============================================================================
L0CaloCandidatesFromRaw::~L0CaloCandidatesFromRaw() {}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode L0CaloCandidatesFromRaw::initialize() {
  StatusCode sc = L0FromRawBase::initialize();
  if ( sc.isFailure() ) return sc;

  if( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_convertTool =
    tool< L0CaloCandidatesFromRawBank >( "L0CaloCandidatesFromRawBank" ) ;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode L0CaloCandidatesFromRaw::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  std::string name = dataLocation( LHCb::L0CaloCandidateLocation::Default ) ;
  std::string nameFull = dataLocation( LHCb::L0CaloCandidateLocation::Full ) ;

  int version = -1 ;
  std::vector<std::vector<unsigned int> > data;

  LHCb::RawBankReadoutStatus readoutStatus( LHCb::RawBank::L0Calo ) ;
  readoutStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::OK ) ;
  readoutStatus.addStatus( 1 , LHCb::RawBankReadoutStatus::Status::OK ) ;

  // Scan the list of input location and select the first existing one.
  // no need to do this any longer, the base class takes care of it
  //std::string rawEventLocation;
  //if ( selectRawEventLocation(rawEventLocation).isFailure() )
  //  return Error("No valid raw event location found",StatusCode::SUCCESS,50);

  LHCb::L0CaloCandidates * outFull = new LHCb::L0CaloCandidates( ) ;
  LHCb::L0CaloCandidates * out = new LHCb::L0CaloCandidates() ;

  if ( writeOnTES() ) {
    put( outFull , nameFull , IgnoreRootInTES ) ;
    put( out, name , IgnoreRootInTES ) ;
    if ( msgLevel( MSG::DEBUG ) )
      debug() << "L0CaloCandidatesFromRawBank Registered output in TES"
              << endmsg ;
  }

  LHCb::RawEvent* rawEvt = findFirstRawEvent();
  if ( !rawEvt )
    return Error("No valid raw event location found",StatusCode::SUCCESS,50);

  const auto& banks = rawEvt -> banks( LHCb::RawBank::L0Calo );
  // check presence of error bank
  const auto& errBanks = rawEvt -> banks( LHCb::RawBank::L0CaloError ) ;

  if (  ( !errBanks.empty() ) )
  {
    for ( const auto* eb : errBanks )
    {
      readoutStatus.addStatus( eb -> sourceID() ,
                               LHCb::RawBankReadoutStatus::Status::ErrorBank ) ;
    }
  }

  if ( 0 == banks.size() )
  {
    Error( "L0Calo Bank has not been found" ).ignore() ;
    readoutStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::Missing ) ;
    readoutStatus.addStatus( 1 , LHCb::RawBankReadoutStatus::Status::Missing ) ;
  }
  else
  {
    int sourceZero( 0 ) , sourceOne( 0 ) ;

    // convert the banks to two arrays of ints.
    data.reserve( banks.size() ) ;
    for ( const auto& bnk : banks )
    {
      if ( LHCb::RawBank::MagicPattern != bnk -> magic() )
      {
        Error( "L0Calo Bank source has bad magic pattern" ).ignore() ;
        readoutStatus.addStatus( bnk -> sourceID() ,
                                 LHCb::RawBankReadoutStatus::Status::Corrupted ) ;
        continue ;
      }

      data.emplace_back(  bnk -> begin< unsigned int >() ,
                          bnk -> end< unsigned int >() ) ;

      if ( 0 == bnk -> sourceID() ) ++sourceZero ;
      else ++sourceOne ;
    }
    // Version of the bank
    version = banks[0] -> version() ;
    if ( 0 == sourceZero )
      readoutStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::Missing ) ;
    if ( 0 == sourceOne )
      readoutStatus.addStatus( 1 , LHCb::RawBankReadoutStatus::Status::Missing ) ;
    if ( 1 < sourceZero )
      readoutStatus.addStatus( 0 , LHCb::RawBankReadoutStatus::Status::NonUnique ) ;
    if ( 1 < sourceOne )
      readoutStatus.addStatus( 1 , LHCb::RawBankReadoutStatus::Status::NonUnique ) ;
  }

  m_convertTool -> convertRawBankToTES( data, outFull , out , version ,
                                        readoutStatus ) ;


  if ( writeOnTES() && statusOnTES() ) {
    // Now put the status on TES also
    LHCb::RawBankReadoutStatuss * statuss =
      getOrCreate< LHCb::RawBankReadoutStatuss , LHCb::RawBankReadoutStatuss >
      ( LHCb::RawBankReadoutStatusLocation::Default ) ;

    LHCb::RawBankReadoutStatus * status =
      statuss -> object( readoutStatus.key() ) ;

    if ( !status ) {
      status = new LHCb::RawBankReadoutStatus( readoutStatus ) ;
      statuss -> insert( status ) ;
    } else {
      // merge both status if already exists
      if ( status -> status() != readoutStatus.status() ) {
        for ( const auto& i : readoutStatus.statusMap() ) {
          status -> addStatus( i.first , i.second ) ;
        }
      }
    }
  }

  if ( writeProcData() ) {
    // Write processor data for L0DU if requested
    // Save the candidates in CaloProcessor data location (for L0DU)
    LHCb::L0ProcessorDatas* L0Calo = new LHCb::L0ProcessorDatas() ;
    put( L0Calo, LHCb::L0ProcessorDataLocation::L0Calo ) ;
    for (auto it = out -> begin() ; it != out -> end() ; ++it ) {
      L0DUBase::Fiber::Type fiber=fiberType((*it)->type());
      if( ! plugFiber( fiber ) )continue;
      L0Candidate cand( (*it) ) ;
      cand.saveCandidate( fiberType( (*it) -> type() ) , L0Calo ) ;
    }
    // add missing data in case of no candidate
    for(unsigned int it=L0DUBase::Fiber::CaloSumEt;it!=L0DUBase::Fiber::Spare1;++it){
      L0DUBase::Fiber::Type ifiber=(L0DUBase::Fiber::Type) it;
      if( L0Calo->object(ifiber) != nullptr)continue;
      if( ! plugFiber( ifiber ) )continue;
      LHCb::L0ProcessorData* temp=new LHCb::L0ProcessorData(ifiber,0x10000);
      L0Calo->add(temp);
    }
  }

  if ( !writeOnTES() ) {
    delete outFull;
    delete out;
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
