#ifndef _UTDetSwitch_H
#define _UTDetSwitch_H

#include <string>

/** 
* Switch the type around (IT - OT and vice versa)
* @param type to switch to
* @param string to switch
*/
namespace UTDetSwitch{

void flip(std::string aType, std::string& aString);

}
#endif
