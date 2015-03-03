// $Id: MEPFragment.h,v 1.3 2006-10-23 09:19:40 frankb Exp $
//====================================================================
//	MEPFragment.h
//--------------------------------------------------------------------
//
//====================================================================
#ifndef MDF_MEPFRAGMENT_H
#define MDF_MEPFRAGMENT_H
// $Header: /afs/cern.ch/project/cvs/reps/lhcb/DAQ/MDF/MDF/MEPFragment.h,v 1.3 2006-10-23 09:19:40 frankb Exp $

// Framework include files
#include "Event/RawBank.h"

/*
 *  LHCb namespace declaration
 */
namespace LHCb  {

  // Forward declarations
  class RawBank;

  /** @class MEPFragment MEPFragment.h Event/MEPFragment.h
    *
    * Structure to describe the data content originating from
    * one tell1 board.
    *
    * Note: A valid construction is only possible with in place
    *       creation on a sufficiently large block of memory
    *       containing the entire fragment size.
    *
    * @author  M.Frank
    * @version 1.0
    *
    */
  class MEPFragment  {
  protected:
    /// Bank definition
    typedef LHCb::RawBank Bank;
    /// Event ID
    unsigned short m_evID;
    /// Fragment size (=size of all banks)
    unsigned short m_size;
    /// First byte of event data
    char           m_bank[4];

  public:
    /// Initializing constructor
    MEPFragment(unsigned short eid, unsigned short siz) 
      : m_evID(eid), m_size(siz)          {                                 }
    /// Default destructor
    ~MEPFragment()                        {                                 }
    /// Access event ID of the fragment
    unsigned short       eventID() const  {  return m_evID;                 }
    /// Set event ID of the fragment
    void setEventID(unsigned short val)   {  m_evID = val;                  }
    /// Length of the fragment
    size_t               size()    const  {  return m_size;                 }
    /// Set event ID of the fragment
    void setSize(unsigned short val)      {  m_size = val;                  }
    /// Access to first bank in the fragment
    char*                start()          {  return m_bank;                 }
    /// Access to first bank in the fragment
    const char*          start()   const  {  return m_bank;                 }
    /// Access to end-iteration over bank
    const char*          end()     const  {  return m_bank+m_size-1;        }
    /// Access to first bank in the fragment
    Bank*                first()   const  {  return (Bank*)m_bank;          }
    /// Access to end-iteration over bank
    const Bank*          last()    const  {  return (Bank*)end();           }
    /// Access to the next bank
    static Bank* next(const Bank* last) {
      // MEP size excludes MEP header size; need to advance 2 shorts more !
      const unsigned int* d = last->data() + last->size()/sizeof(int);
      return (Bank*)(last->size()%sizeof(int)==0 ? d : d+1);
    }
    /// Object size ithout variable buffer
    static size_t sizeOf()  {  return sizeof(MEPFragment)-4*sizeof(char);     }
  };

  /** Basic run information from ODIN bank.
    */
  struct RunInfo  {
    unsigned int  Run;
    unsigned int  Orbit;
    long long int L0ID;
    long long int GPSTime;
    unsigned      detStatus   : 24;
    unsigned      errors      :  8;
    unsigned      bunchID     : 12;
    unsigned      pad0        :  4;
    unsigned      triggerType :  3;
    unsigned      readoutType :  2;
    unsigned      forceBit    :  1;
    unsigned      bxType      :  2;
    unsigned      bunchCurrent:  8;
  };
}      // end namespace LHCb
#endif // MDF_MEPFRAGMENT_H
