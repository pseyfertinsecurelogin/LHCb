// $Id: SimpleValidity.h,v 1.1 2006-04-20 14:39:56 ranjard Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.3  2006/02/01 19:39:10  marcocle
// use Gaudi::Time instead of ITime+TimePoint
//
// Revision 1.2  2005/04/22 13:10:41  marcocle
// Follow (pending) change in GaudiKernel/IValidity.
// Improvements in ValidDataObject and LogVolBase. (see doc/release.notes)
//
// Revision 1.1  2001/11/18 15:32:45  ibelyaev
//  update for Logical Assemblies
//
// ============================================================================
#ifndef DETDESC_SIMPLEVALIDITY_H
#define DETDESC_SIMPLEVALIDITY_H 1
// Include files
#include "GaudiKernel/IValidity.h"

/** @class SimpleValidity SimpleValidity.h
 *
 *  The simplest implementation of IValidity interface
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   17/11/2001
 */

class SimpleValidity:
  public  virtual  IValidity
{
public:

  /** standard constructor
   */
  SimpleValidity();

  /** standard constructor
   *  @param since "since" time for validity range
   *  @param till  "till"  time for validity range
   */
  SimpleValidity( const Gaudi::Time& since    ,
                  const Gaudi::Time& till     );

  /** (explicit) constructor from other
   *   IValidity object
   *   @param copy another IValidity object
   */
  explicit
  SimpleValidity( const IValidity&      copy );

  /** copy constructor  (deep copy)
   *  @param copy another IValidity object
   */
  SimpleValidity( const SimpleValidity& copy );

  /** assignement from any IValidity object
   *  @param copy another IValidity object
   */
  SimpleValidity& operator=( const IValidity&      copy );

  /** assignement (deep copy)
   *  @param copy another IValidity object
   */
  SimpleValidity& operator=( const SimpleValidity& copy );

  /** destructor
   */
  virtual ~SimpleValidity();

  /** is the Object valid? (it can be always invalid)
   *  @return true if object valid
   */
  bool  isValid    () const override;

  /** is the Object valid for a given time?
   *  @param  time  tiemto be checked
   *  @return true if objetc is valid for given time
   */
  bool  isValid    ( const Gaudi::Time&  time ) const override;

  /** since what time the Objest is valid?
   *  @return time "since"
   */
  inline const Gaudi::Time&
  validSince () const override { return m_since; }

  /** till  what time the Object is Valid?
   *  @return time "till"
   */
  inline const Gaudi::Time&
  validTill  () const override { return m_till ; }

  /// set the validity range of the Object
  void
  setValidity       ( const Gaudi::Time& since ,
                      const Gaudi::Time& till  ) override;
  /// set the validity time of the Object
  void
  setValiditySince  ( const Gaudi::Time& since ) override;

  /// set the validity time of the Object
  void
  setValidityTill   ( const Gaudi::Time& till  ) override;

private:

  Gaudi::Time m_since ;
  Gaudi::Time m_till  ;

};

// ============================================================================
// The End
// ============================================================================
#endif // DETDESC_SIMPLEVALIDITY_H
// ============================================================================
