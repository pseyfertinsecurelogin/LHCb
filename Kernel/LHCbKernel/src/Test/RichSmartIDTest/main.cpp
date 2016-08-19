
#include <iostream>
#include "Kernel/RichSmartID.h"
#include "Kernel/RichSmartID32.h"
#include "Kernel/LHCbID.h"

using namespace std;

int main ( int /*argc*/, char** /*argv*/ )
{

  bool OK = true;

  //const LHCb::RichSmartID nullID64;
  //std::cout << "Null64 : " << nullID64 << std::endl;

  //const LHCb::RichSmartID32 nullID32;
  //std::cout << "Null32 : " << nullID32 << std::endl;

  // test 32 -> 64 bit compatibility
  for ( unsigned int irich = 0; irich <= LHCb::RichSmartID32::HPD::MaxRich; ++irich )
  {
    const Rich::DetectorType rich = (Rich::DetectorType)irich;
    for ( unsigned int ipanel = 0; ipanel <= LHCb::RichSmartID32::HPD::MaxPanel; ++ipanel )
    {
      const Rich::Side panel = (Rich::Side)ipanel;
      for ( unsigned int pdrow = 0; pdrow <= LHCb::RichSmartID32::HPD::MaxPDNumInCol; ++pdrow )
      {
        for ( unsigned int pdcol = 0; pdcol <= LHCb::RichSmartID32::HPD::MaxPDCol; ++pdcol )
        {
          for ( unsigned int pixrow = 0; pixrow <= LHCb::RichSmartID32::HPD::MaxPixelRow; ++pixrow )
          {
            for ( unsigned int pixcol = 0; pixcol <= LHCb::RichSmartID32::HPD::MaxPixelCol; ++pixcol )
            {
              for ( unsigned int subpix = 0; subpix <= LHCb::RichSmartID32::HPD::MaxPixelSubRow; ++subpix )
              {

                // Make a 32 bit ID
                const LHCb::RichSmartID32 id32( rich,panel,
                                                pdrow,pdcol,
                                                pixrow,pixcol,subpix,
                                                LHCb::RichSmartID32::HPDID );
                // make a 64 bit version from the 32 bit version
                const LHCb::RichSmartID id64( id32 );
                // Check values
                if ( id32.rich()        != id64.rich()        ||
                     id32.panel()       != id64.panel()       ||
                     id32.pdNumInCol()  != id64.pdNumInCol()  ||
                     id32.pdCol()       != id64.pdCol()       ||
                     id32.pixelRow()    != id64.pixelRow()    ||
                     id32.pixelCol()    != id64.pixelCol()    ||
                     id32.pixelSubRow() != id64.pixelSubRow() )
                {
                  cerr << "Problem with RichSmartID 32<->64 bit comparison :" << std::endl;
                  cerr << "  32 bit : " << id32 << endl;
                  cerr << "  64 bit : " << id64 << endl;
                  OK = false;
                }

              }
            }
          }
        }
      }
    }
  }

  for ( unsigned int irich = 0; irich <= LHCb::RichSmartID::HPD::MaxRich; ++irich )
  {
    const Rich::DetectorType rich = (Rich::DetectorType)irich;
    for ( unsigned int ipanel = 0; ipanel <= LHCb::RichSmartID::HPD::MaxPanel; ++ipanel )
    {
      const Rich::Side panel = (Rich::Side)ipanel;
      for ( unsigned int pdrow = 0; pdrow <= LHCb::RichSmartID::HPD::MaxPDNumInCol; ++pdrow )
      {
        for ( unsigned int pdcol = 0; pdcol <= LHCb::RichSmartID::HPD::MaxPDCol; ++pdcol )
        {
          for ( unsigned int pixrow = 0; pixrow <= LHCb::RichSmartID::HPD::MaxPixelRow; ++pixrow )
          {
            for ( unsigned int pixcol = 0; pixcol <= LHCb::RichSmartID::HPD::MaxPixelCol; ++pixcol )
            {
              for ( unsigned int subpix = 0; subpix <= LHCb::RichSmartID::HPD::MaxPixelSubRow; ++subpix )
              {

                // Make a test smart ID
                const LHCb::RichSmartID id( rich,panel,
                                            pdrow,pdcol,
                                            pixrow,pixcol,subpix,
                                            LHCb::RichSmartID::HPDID );

                // Check values
                if ( rich   != id.rich()       ||
                     panel  != id.panel()      ||
                     pdrow  != id.pdNumInCol() ||
                     pdcol  != id.pdCol()      ||
                     pixrow != id.pixelRow()   ||
                     pixcol != id.pixelCol()   ||
                     subpix != id.pixelSubRow() )
                {
                  cerr << "Problem with RichSmartID " << id << endl;
                  OK = false;
                }

                // // Test conversion to and from an LHCbID
                // {
                //   const LHCb::LHCbID lID( id );
                //   const LHCb::RichSmartID nid( lID.richID() );
                //   if ( nid.key() != id.key() )
                //   {
                //     cerr << "Problem with RichSmartID <-> LHCbID conversion" << endl;
                //     cerr << "  Original  ID : " << id  << endl;
                //     cerr << "  Converted ID : " << nid << endl;
                //     OK = false;
                //   }
                // }

                // Test conversion to/from various int types
                {
                  //const           int isHPD  = id;
                  //const unsigned  int iuHPD  = id;
                  const          long ilsHPD = id;
                  const unsigned long iluHPD = id;
                  //const LHCb::RichSmartID s_id(isHPD), u_id(iuHPD);
                  const LHCb::RichSmartID sl_id(ilsHPD), ul_id(iluHPD);
                  // if ( id != s_id )
                  // {
                  //   cerr << "Problem with RichSmartID <-> signed int conversion" << endl;
                  //   cerr << "  Original  ID : " << id  << endl;
                  //   cerr << "  Converted ID : " << s_id << endl;
                  //   OK = false;
                  // }
                  // if ( id != u_id )
                  // {
                  //   cerr << "Problem with RichSmartID <-> unsigned int conversion" << endl;
                  //   cerr << "  Original  ID : " << id  << endl;
                  //   cerr << "  Converted ID : " << u_id << endl;
                  //   OK = false;
                  // }
                  if ( id != sl_id )
                  {
                    cerr << "Problem with RichSmartID <-> signed long int conversion" << endl;
                    cerr << "  Original  ID : " << id  << endl;
                    cerr << "  Converted ID : " << sl_id << endl;
                    OK = false;
                  }
                  if ( id != ul_id )
                  {
                    cerr << "Problem with RichSmartID <-> unsigned long int conversion" << endl;
                    cerr << "  Original  ID : " << id  << endl;
                    cerr << "  Converted ID : " << ul_id << endl;
                    OK = false;
                  }
                }

              }
            }
          }
        }
      }
    }
  }

  for ( unsigned int irich = 0; irich <= LHCb::RichSmartID::MaPMT::MaxRich; ++irich )
  {
    const Rich::DetectorType rich = (Rich::DetectorType)irich;
    for ( unsigned int ipanel = 0; ipanel <= LHCb::RichSmartID::MaPMT::MaxPanel; ++ipanel )
    {
      const Rich::Side panel = (Rich::Side)ipanel;
      for ( unsigned int pdrow = 0; pdrow <= LHCb::RichSmartID::MaPMT::MaxPDNumInCol; ++pdrow )
      {
        for ( unsigned int pdcol = 0; pdcol <= LHCb::RichSmartID::MaPMT::MaxPDCol; ++pdcol )
        {
          for ( unsigned int pixrow = 0; pixrow <= LHCb::RichSmartID::MaPMT::MaxPixelRow; ++pixrow )
          {
            for ( unsigned int pixcol = 0; pixcol <= LHCb::RichSmartID::MaPMT::MaxPixelCol; ++pixcol )
            {

              // Make a test smart ID
              const LHCb::RichSmartID id( rich,panel,
                                          pdrow,pdcol,
                                          pixrow,pixcol,
                                          LHCb::RichSmartID::MaPMTID );

              // Check values
              if ( rich   != id.rich()       ||
                   panel  != id.panel()      ||
                   pdrow  != id.pdNumInCol() ||
                   pdcol  != id.pdCol()      ||
                   pixrow != id.pixelRow()   ||
                   pixcol != id.pixelCol()    )
              {
                cerr << "Problem with RichSmartID " << id << endl;
                OK = false;
              }

              // // Test conversion to and from an LHCbID
              // {
              //   const LHCb::LHCbID lID( id );
              //   const LHCb::RichSmartID nid( lID.richID() );
              //   if ( nid.key() != id.key() )
              //   {
              //     cerr << "Problem with RichSmartID <-> LHCbID conversion" << endl;
              //     cerr << "  Original  ID : " << id  << endl;
              //     cerr << "  Converted ID : " << nid << endl;
              //     OK = false;
              //   }
              // }

              // Test conversion to/from various int types
              {
                //const           int isHPD  = id;
                //const unsigned  int iuHPD  = id;
                const          long ilsHPD = id;
                const unsigned long iluHPD = id;
                //const LHCb::RichSmartID s_id(isHPD), u_id(iuHPD);
                const LHCb::RichSmartID sl_id(ilsHPD), ul_id(iluHPD);
                // if ( id != s_id )
                // {
                //   cerr << "Problem with RichSmartID <-> signed int conversion" << endl;
                //   cerr << "  Original  ID : " << id  << endl;
                //   cerr << "  Converted ID : " << s_id << endl;
                //   OK = false;
                // }
                // if ( id != u_id )
                // {
                //   cerr << "Problem with RichSmartID <-> unsigned int conversion" << endl;
                //   cerr << "  Original  ID : " << id  << endl;
                //   cerr << "  Converted ID : " << u_id << endl;
                //   OK = false;
                // }
                if ( id != sl_id )
                {
                  cerr << "Problem with RichSmartID <-> signed long int conversion" << endl;
                  cerr << "  Original  ID : " << id  << endl;
                  cerr << "  Converted ID : " << sl_id << endl;
                  OK = false;
                }
                if ( id != ul_id )
                {
                  cerr << "Problem with RichSmartID <-> unsigned long int conversion" << endl;
                  cerr << "  Original  ID : " << id  << endl;
                  cerr << "  Converted ID : " << ul_id << endl;
                  OK = false;
                }
              }

            }
          }
        }
      }
    }
  }

  if ( !OK )
  {
    cerr << "RichSmartID tests FAILED" << endl;
  }
  else
  {
    cout << "RichSmartID tests SUCCESSFUL" << endl;
  }

  return ( OK ? 0 : 1 );
}
