#ifndef _FTTell40ID_H
#define _FTTell40ID_H 1

/** @class FTTell40ID FTTell40ID.h "STDAQ/FTTell40ID.h"
 *
 *  Class to describe a FTTell40ID
 *
 *  @author L.Henry
 *  @date   12/12/2017
 */

#include <iostream>

class FTTell40ID final {
	
 public:
	/// constructor with strip and waferIndex
	FTTell40ID(unsigned int aRegion,
			   unsigned int aSubID){
		
		m_id = (aRegion<<regionBits) +
			(aSubID<<subIDBits);
	}
	
	explicit FTTell40ID(unsigned int id):
	m_id(id){
	}
	
	/// Default Constructor
	FTTell40ID() = default;
	
	/// wafer
	unsigned int region() const;
	
	/// strip
	unsigned int subID() const;
	
	/// cast
	//  operator int() const;
	
	/// comparison equality
	friend bool operator==(const FTTell40ID& lhs, const FTTell40ID& rhs)
	{ return lhs.id() == rhs.id(); }
	
	/// comparison <
	friend bool operator<(const FTTell40ID& lhs, const FTTell40ID& rhs)
	{ return lhs.id() < rhs.id(); }
	
	/// Retrieve IT Channel ID
	unsigned int id() const;
	
	/// Operator overloading for stringoutput
	friend std::ostream& operator<< (std::ostream& s, const FTTell40ID& obj)
		{
			return obj.fillStream(s);
		}
	
	// Fill the ASCII output stream
	std::ostream& fillStream(std::ostream& s) const;
	
	/** print method for python Not needed in C++ */
	std::string toString() const;
	
	enum General { nullBoard =  0x0000ffff};
	
 private:
	
	enum bits  {subIDBits = 0,  regionBits = 5}; /// Enumeration to store the bit packing offsets
	enum masks {subIDMask = 0x0000001f, regionMask = 0x000000e0};
	
	unsigned int m_id = 0; /// FTTell40ID
};

	
#include <sstream>
#include <string>

inline std::string FTTell40ID::toString() const{
	std::ostringstream o;
	fillStream(o);
	return o.str();
}


inline unsigned int FTTell40ID::id() const
{
  return m_id;
}

inline unsigned int FTTell40ID::region() const
{
	return (m_id & regionMask) >> regionBits;
}

inline unsigned int FTTell40ID::subID() const
{
	return ((m_id & subIDMask) >> subIDBits);
}

inline std::ostream& FTTell40ID::fillStream(std::ostream& s) const
{
  s << "{ "
    << " FTTell40ID:\t" << id() << std::endl
    << " region:\t"     << region() << std::endl
    << " subID:\t"     << subID();
  s << " } ";

  return s;
}

#endif // _FTTell40ID_H


