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
// Include files
#include <iomanip>

// local
#include "OfflineRateLimiter.h"
//-----------------------------------------------------------------------------
// Implementation file for class : OfflineRateLimiter
//
// 2011-08-05 : Patrick Koppenburg
//-----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OfflineRateLimiter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
OfflineRateLimiter::OfflineRateLimiter( const std::string& name,
                                        ISvcLocator* pSvcLocator)
  : OfflineDeterministicPrescaler ( name , pSvcLocator )
{
  declareProperty("HltLimiter",m_hltLimiter = "Hlt1MBNoBiasODINFilter",
                  "Reference Hlt rate limiter");
  declareProperty("Rate",m_rate = 0.,"Rate to achieve [Hz]");
  declareProperty("UseCondition", m_useCondDB = false);
  declareProperty("FallBack", m_fallback = true);
  declareProperty("Prescaler", m_prescaler = "Hlt1MBNoBiasPreScaler","Name of Prescaler");
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode OfflineRateLimiter::initializeOnFirstEvent()
{
  StatusCode sc = StatusCode::SUCCESS ;
  if (m_initialized) return sc;
  if (UNLIKELY( msgLevel(MSG::DEBUG) )) debug() << "==> Initialize" << endmsg;
  m_initialized = true ;
  if ( m_rate<0) Exception("Negative Rate requested");
  if ( m_useCondDB )
  {
    m_condTriggerTool = tool<RateFromCondDB>("RateFromCondDB",this);
    if (!m_condTriggerTool) 
    {
        warning() << "Failed to initialise RateFromCondDB. This is expected on MC." << endmsg ;
        m_useCondDB = false ;
    } else {
      if (m_condTriggerTool->initializeCondDB()){
        if ( ""!= m_prescaler)
        {  
          m_tckReader = tool<IRateFromTCK>("RateFromTCK",this); // make it private
          sc =  m_tckReader->runUpdate();
          if (!sc) {
            warning() << "Failed to update condDB for RateFromTCK. Will assume there is no preccale." << endmsg ;
            release ( m_tckReader );
            m_tckReader = nullptr;
          }
        }
      } else if (m_fallback) {
        info() << "Failed to get rate from condition database. This is normal for <2012." << endmsg ;
        info() << "Fill fallback on TCK" << endmsg ;
        release(m_condTriggerTool);
        m_useCondDB = false ;
      } else{
        Exception("Failed to get rate from condition database and no fallback") ;
      }
    }
  }
  // this one I need anyway as fallback
  if ( !m_useCondDB || m_fallback ){
    m_tckReader = tool<IRateFromTCK>("RateFromTCK",this); // make it private
    if (!m_tckReader) return StatusCode::FAILURE;
    sc =  m_tckReader->runUpdate();
    if (!sc) {
      warning() << "Failed to update condDB for RateFromTCK. This is OK on MC. Setting rate ot 0." << endmsg ;
      m_useCondDB = false ;
      m_rate = 0; 
    }
  }
  updateRate() ;
  
  return StatusCode::SUCCESS;
}
//=============================================================================
// Not an Incident
//=============================================================================
bool OfflineRateLimiter::handleTCK() 
{ 
  if (UNLIKELY( msgLevel(MSG::DEBUG) )) debug() << "==> handleTCK "  << endmsg;
  if (!m_tckReader) Exception("TCK Reader is NULL in handleTCK");
  const unsigned int newTCK = m_tckReader->getTCK() ;
  if ( m_tck == newTCK ) return false ;
  
  if (UNLIKELY( msgLevel(MSG::DEBUG) )) debug() << " changed TCK from " << std::hex 
                                                << m_tck << " to " <<  newTCK << endmsg;
  m_tck = newTCK ;
  return true ;
}
//=============================================================================
// 
//=============================================================================
void OfflineRateLimiter::updateRate()
{
  const double triggerRate = (m_storedRate*m_storedPrescale);
  const double newacc = (triggerRate>0)?m_rate/triggerRate:0. ;
  if (UNLIKELY( msgLevel(MSG::DEBUG))) debug() << "==> updateRate " << newacc << endmsg;  
  if ( m_accFrac != newacc )
  {    
    info() << "Rate is now " << m_storedRate 
           << " Hz and precale " << m_storedPrescale << " (=" << triggerRate << " Hz)"
           << " -> Need to reduce by factor " << newacc
           << " to reach target "  << m_rate << " Hz." << endmsg ;
    m_accFrac = newacc ;
    update(); // update in baseclass
  }
}
//=============================================================================
// 
//=============================================================================
void OfflineRateLimiter::updateRateFromTCK()
{
  if ( handleTCK() )
  {
    const double randomRate = m_tckReader->rateFromTCK(m_hltLimiter) ;
    if ( randomRate <= 0 )
    {
      warning() << "Rate of " << m_hltLimiter << " in TCK " << std::hex << m_tck << " is " 
                << randomRate << " Hz" << endmsg ;
    }
    m_storedRate = randomRate ;
    updateRate();
  }
}
//=============================================================================
// 
//=============================================================================
void OfflineRateLimiter::updatePrescaleFromTCK()
{
  if ( handleTCK() )
  {
    const double prescale = m_tckReader->prescaleFromTCK(m_prescaler) ;
    if (  prescale <= 0 )
    {
      warning() << "Prescale of " << m_prescaler << " in TCK " << std::hex << m_tck << " is " 
                << prescale << endmsg ;
    }
    m_storedPrescale = prescale ;
    updateRate();
  }
}
//=============================================================================
// Not an Incident
//=============================================================================
void OfflineRateLimiter::updateRateFromCondDB() 
{ 
  if (UNLIKELY( msgLevel(MSG::DEBUG) )) debug() << "==> updateRateFromCondDB "  << endmsg;
  if (!m_condTriggerTool) Exception("CondDB Reader is NULL in updateRateFromCondDB");
  
  const double randomRate = m_condTriggerTool->getRate() ;
  if ( randomRate > 0 )
  {
    m_storedRate = randomRate ;
    if ( m_tckReader ) { updatePrescaleFromTCK(); }
    else               { updateRate();            }
  } 
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode OfflineRateLimiter::execute() 
{
  //  somewhere here get rate
  if (!initializeOnFirstEvent()) return StatusCode::FAILURE;
  if (UNLIKELY( msgLevel(MSG::DEBUG) )) debug() << "==> Execute "  << endmsg;
  if ( m_useCondDB ) { updateRateFromCondDB(); }                  // Try via CondDB
  else               { updateRateFromTCK(); }
  const bool acc = accept();
  setFilterPassed(acc);
  *m_counter += acc;
  if (msgLevel(MSG::DEBUG)) debug() << (acc?"Accepted":"Rejected") << endmsg ;
  return StatusCode::SUCCESS;
}

