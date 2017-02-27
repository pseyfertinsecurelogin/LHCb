#ifndef LINKER_LINKEDFROMKEY_H 
#define LINKER_LINKEDFROMKEY_H 1

// Include files
#include "Linker/LinkedFrom.h"

/** @class LinkedFromKey LinkedFromKey.h Linker/LinkedFromKey.h
 *  Simple helper class to handle LinkedFrom with no source, i.e. key linker
 *
 *  @author Olivier Callot
 *  @date   2008-03-14
 */

template <class TARGET, class KEY=int> 
using LinkedFromKey = LinkedFrom<ContainedObject, TARGET, KEY>;

#endif // LINKER_LINKEDFROMKEY_H
