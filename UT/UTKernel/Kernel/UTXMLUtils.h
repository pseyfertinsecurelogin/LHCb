#ifndef UTXMLUtils_H
#define UTXMLUtils_H 1

/** @namespace UTXMLUtils UTXMLUtils.h
 *
 *  class to help writing XML comments
 *
 *
 *  @author A Beiter (based on code by M Needham)
 *  @date   2018-09-04
 */


#include <string>
#include <sstream>

namespace UT
{
  namespace XMLUtils {


    /** add a comment for the author
     *  <!-- Author: author --!> */
    void authorComment(std::ostringstream& stream,
                       const std::string& author) ;

    /** add a comment for the Version Tag
    <!-- Version: tag --!> */
    void tagComment(std::ostringstream& stream,
                    const std::string& tag );


    /** add a comment for the description
     *  <!-- Descrption: desc --!> */
    void descriptionComment(std::ostringstream& stream,
                            const std::string& description) ;

    /** Pipe all the comments together
     *  <!-- Author: author --!>
     *  <!-- Version: tag --!>
     *  <!-- Descrption: desc --!> */
    void fullComment(std::ostringstream& stream,
                     const std::string& author,
                     const std::string& tag,
                     const std::string desc) ;

    /** Generic comment
     *  <!-- Name value --!>
     */
    void comment(std::ostringstream& stream,
                const std::string& name,
                const std::string& value);

    /** remove nasty characters
    The characters , [ ] mapped to whitespace
    / to \
    **/
    void replaceChars(std::string& conString);

    /** replace in with out in conString */
    void replace(std::string& conString,
                 std::string in,
		 std::string out );

  }
}

#endif // UT::XMLUtils
