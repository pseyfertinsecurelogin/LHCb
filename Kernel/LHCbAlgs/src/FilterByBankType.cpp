// $Id: FilterByBankType.cpp,v 1.1 2009-03-09 21:18:07 jvantilb Exp $
// Include files 

#include <boost/regex.hpp>
#include <algorithm>
#include <functional>

// From DAQEvent
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
// local
#include "FilterByBankType.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"

class ISvcLocator;

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : FilterByBankType
//
// 2009-03-06 : Jeroen van Tilburg
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( FilterByBankType )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FilterByBankType::FilterByBankType( const std::string& name,
                                    ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  m_bankNames.push_back(".*Error");
  
  declareProperty("InputLocation", m_inputLocation = RawEventLocation::Default);
  declareProperty("BankNames", m_bankNames );
  declareProperty("PassSelectedEvents", m_passSelect = true );
}

StatusCode FilterByBankType::initialize()
{
  // Gaudi initialize
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  
  // Loop over the list of possible BankTypes
  info() << ((m_passSelect) ? "Selecting" : "Ignoring" ) 
         << " events with banks: ";
  for ( unsigned int iBank = 0; iBank < RawBank::LastType; ++iBank ) 
  {
    const auto bankName = RawBank::typeName( RawBank::BankType(iBank) );
    // make an enum vector from the string vector of bank names
    for ( const auto & BankName : m_bankNames )
    {
      // Use the regular expression
      boost::regex e(BankName);
      if( boost::regex_match( bankName, e) )
      {
        m_bankTypes.push_back( RawBank::BankType(iBank) );
        info() << bankName << "(" << iBank << ")  " ;
      }
    }
  }
  info() << endmsg;
  
  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FilterByBankType::execute()
{
  // Initialize the select event flag
  bool selectEvent = false;
  
  // Reset the filter 
  setFilterPassed(!m_passSelect);

  // Get the raw data
  const auto * raw = get<RawEvent>( m_inputLocation );

  // Loop over the bank types
  auto iBankType = m_bankTypes.begin();
  while( !selectEvent && iBankType < m_bankTypes.end() ) 
  {
    
    // Get the bank in the RawEvent
    const auto & bank = raw->banks( *iBankType );

    // If bank exist mark the event
    if ( bank.size() > 0 ) 
    {
      selectEvent = true;

      // Make some printout if bank is found
      if (msgLevel(MSG::DEBUG))
      {
        debug() << "Found " << bank.size() << " bank(s) of type " 
                << *iBankType << " (" <<  (bank.front())->typeName() << ")." 
                << endmsg;
      }      
    }

    // Loop over bank types
    ++iBankType;

  } // end of loop over bank types  

  // Change the filter-passed-flag when event is selected
  if( selectEvent ) setFilterPassed( m_passSelect );

  return StatusCode::SUCCESS;
}
//=============================================================================
