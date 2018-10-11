/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef TCK_H
#define TCK_H
#include "boost/operators.hpp"
#include "boost/format.hpp"

class StatusCode;

class TCK final : public boost::equality_comparable<TCK>,
                         boost::equality_comparable2<TCK,unsigned int> {
public:
    TCK() = default;
    explicit TCK(unsigned int i) : m_unsigned(i) { set(i); }
    explicit TCK(std::string s) { set(s); }
    friend bool operator<(const TCK& lhs, const TCK& rhs)
    { return lhs.m_unsigned  < rhs.m_unsigned; }
    friend bool operator==(const TCK& lhs, const TCK& rhs)
    { return lhs.m_unsigned == rhs.m_unsigned; }
    friend bool operator==(const TCK& lhs, unsigned int rhs)
    { return lhs.m_unsigned == rhs; }
    friend bool operator==(unsigned int& lhs, const TCK& rhs)
    { return lhs == rhs.m_unsigned; }
    TCK& operator++() { return set( ++m_unsigned ); }
    const std::string&  str() const { return m_stringRep; }
    unsigned int uint() const { return m_unsigned;  }
    TCK& normalize() {
        // if upper bit set, ignore L0... in that case, the L0 TCK is generated by PVSS
        // and not by us, and hence we only know it with the L0 part blanked out...
        if ((uint() & 0x80000000)!=0 ) maskL0();
        return *this;
    }
    TCK& maskL0() { set( uint() & 0xFFFF0000 ); return *this; }
    TCK& set(const std::string& s);
    TCK& set(unsigned i) {
        m_unsigned = i;
        m_stringRep = boost::str( boost::format("0x%08x")%i ) ;
        return *this;
    }
    bool valid() const { return m_unsigned != 0 ; }

    friend std::ostream& operator<<(std::ostream& os, const TCK& tck)
    { return os << tck.str(); }

    // allow Gaudi::Property<TCK> :
    friend std::ostream& toStream(const TCK& tck, std::ostream& os)
    { return os << '\'' << tck << '\''; }
    friend StatusCode parse(TCK& result, const std::string& input );

private:
    std::string m_stringRep;
    unsigned int m_unsigned = 0;
};




#endif
