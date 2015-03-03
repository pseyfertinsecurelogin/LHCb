
//--------------------------------------------------------------------------------
/** @file RichObjPtn.h
 *
 *  Header file for RICH utility class : Rich::ObjPtn
 *
 *  CVS Log :-
 *  $Id: RichObjPtn.h,v 1.5 2007-03-09 17:58:13 jonrob Exp $
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   13/11/2005
 */
//--------------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHOBJPTN_H
#define RICHKERNEL_RICHOBJPTN_H 1

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class ObjPtn RichObjPtn.h RichKernel/RichObjPtn.h
   *
   *  A utility class providing on demand object creation and some auto pointer
   *  like behaviour
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   13/11/2005
   */
  //--------------------------------------------------------------------------------

  template < class TYPE >
  class ObjPtn
  {

  public:

    /// Default Constructor
    ObjPtn() : m_obj(NULL) {}

    /** Constructor from pointer to the underlying object
     *
     *  @param obj Pointer to object to use as underlying data object
     *
     *  @attention Using this method, RichObjPtn takes ownership of
     *  the object pointed at by obj. In particular, it is deleted
     *  when this RichObjPtn goes out of scope or is deleted itself.
     *  Consequently users should NOT manually delete objects they
     *  pass to a RichObjPtn.
     */
    explicit ObjPtn( TYPE * obj ) : m_obj(obj) {}

    /// Destructor
    ~ObjPtn() { delete m_obj; }

  private:

    /// Check if new object is needed
    inline TYPE* checkObj() const
    {
      if ( NULL == m_obj ) { m_obj = new TYPE(); }
      return m_obj;
    }

  public:

    /// Check if an object is defined
    inline bool objectExists()      const { return NULL != m_obj; }

    /// Dereference operator to const object
    inline const TYPE* operator->() const { return checkObj(); }

    /// Dereference operator
    inline TYPE* operator->()             { return checkObj(); }

    /// Simple const access method
    inline const TYPE* object()     const { return checkObj(); }

    /// Simple access method
    inline TYPE* object()                 { return checkObj(); }

    /// Overload output to ostream
    friend inline std::ostream& operator << ( std::ostream& s,
                                              const Rich::ObjPtn<TYPE>& ptn )
    {
      return s << *(ptn.object());
    }

  private:

    /// Pointer to the data object
    mutable TYPE * m_obj;

  };

}

#endif // RICHKERNEL_RICHOBJPTN_H
