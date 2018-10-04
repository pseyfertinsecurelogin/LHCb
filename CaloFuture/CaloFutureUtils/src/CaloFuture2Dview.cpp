// Include files 

// from Gaudi
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IProfile2D.h"
#include "AIDA/IProfile1D.h"
#include "GaudiUtils/Aida2ROOT.h"
// from LHCb
#include "Kernel/CaloCellCode.h"
// local
#include "CaloFutureUtils/CaloFuture2Dview.h"

//------------------------------------------------------------------------------
// Implementation file for class : CaloFuture2Dview
//
// 2008-01-17 : Olivier Deschamps
//------------------------------------------------------------------------------

// Declaration of the Algorithm Factory
//DECLARE_COMPONENT( CaloFuture2Dview );

//==============================================================================

namespace {
  // Cache the array of cellSize given the subdetector.
  std::vector<double> cell_sizes(const CaloFuture2Dview::CaloFutureParams& cp){
    auto result = std::vector<double>();
    for( const auto& cell: cp.refCell ){
      result.push_back( cp.calo->cellSize(cell) );
    };
    return result;
  }
}


//==============================================================================
// Standard constructor, initializes variables
//==============================================================================

CaloFuture2Dview::CaloFuture2Dview( const std::string& name,
                        ISvcLocator* pSvcLocator)
  : GaudiHistoAlg ( name , pSvcLocator )
{
  declareProperty( "OneDimension"   , m_1d = false);            // 1D histo (value versus CaloCellID) default is 2D view
  declareProperty( "Profile"        , m_profile = false);       // 1D histo profile type ?
  declareProperty( "Bin1D"          , m_bin1d = 16384);         // 1D histo binning (default : full 14 bits dynamics)
  declareProperty( "GeometricalView", m_geo = true);            // 2D :  true : geometrical (x,y) | false : readout (FEB,channel)
  declareProperty( "Threshold"      , m_threshold = -256.);     // threshold on the input value (bin weight)
  declareProperty( "PinView"        , m_pin = false );          // produce a separate view for PIN channels (geometrical view only)
  declareProperty( "Offset"         , m_offset = 0.0);          // bin weight =  (value + offset)
  declareProperty( "ActualSize"     , m_dim = true );           // true: (x,y) | false : (col,row) (geometrical view only)
  declareProperty( "L0ClusterView"  , m_l0 = false );           // bin weight on 2x2 cluster (geometrical view only)
  declareProperty( "EnergyWeighted" , m_energyWeighted = true); // true : bin weight = energy | false :  1
  declareProperty( "Flux"           , m_flux = false );         // bin weight is normalized to the cell area (for both views)
  declareProperty( "SplitAreas"     , m_split = false );        // produce one view per area (for geometrical view only)
  declareProperty( "xLabelOptions"  , m_lab = "v" );            // Crate-FEB text label on Xaxis (readout view only)
  declareProperty( "ProfileError"   , m_prof = "" );
  setHistoDir( name );
}


//==============================================================================

StatusCode CaloFuture2Dview::initialize() {
  StatusCode sc = GaudiHistoAlg::initialize();
  if( sc.isFailure() ) return sc;

  if( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  //protection against splitting when non-goemetrical view or 1D is requested.
  if( !m_geo ) m_split = false;
  if( m_1d ) m_split = false; 

  // Set up the parameters
  m_caloParams[0] = {
    getDetIfExists<DeCalorimeter>( DeCalorimeterLocation::Spd ),
    64, 32, 6, 0, 127, {
      LHCb::CaloCellID(0, 0, 6 , 0), // outer
      LHCb::CaloCellID(0, 1, 12, 0), // middle
      LHCb::CaloCellID(0, 2, 14, 8)  // inner
    }, {}, {}};
  
  m_caloParams[1] = { 
    getDetIfExists<DeCalorimeter>( DeCalorimeterLocation::Prs ),
    64, 32, 6, 0, 127, {
      LHCb::CaloCellID(1, 0, 6 , 0),  // outer
      LHCb::CaloCellID(1, 1, 12, 0),  // middle
      LHCb::CaloCellID(1, 2, 14, 8)   // inner
    }, {}, {}};

  m_caloParams[2] = {
    getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal ),
    32, 32, 6, 128, 351, {
      LHCb::CaloCellID(2, 0, 6 , 0), // outer
      LHCb::CaloCellID(2, 1, 12, 0), // middle
      LHCb::CaloCellID(2, 2, 14, 8)  // inner    
    }, {}, {}};

  m_caloParams[3] = {
    getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal ),
    32, 16, 2, 352, 415, { 
      LHCb::CaloCellID(3, 0, 3 ,0),
      LHCb::CaloCellID(3, 1, 2 ,0)
    }, {}, {}};

  // Check if missing, once.
  if (m_caloParams[0].calo == nullptr)
    Warning("Spd Element does not exists !", StatusCode::SUCCESS).ignore();
  if (m_caloParams[1].calo == nullptr)
    Warning("Prs Element does not exists !", StatusCode::SUCCESS).ignore();

  // Init the derived values
  // ? no need for m_calo->YtoXratio() ?
  for( auto& cp: m_caloParams ){
    if (cp.calo != nullptr) cp.xsize = cp.ysize = cell_sizes(cp);
  }

  return StatusCode::SUCCESS;
}

//==============================================================================
 
void CaloFuture2Dview::bookCaloFuture2D(const HistoID& unit, const std::string title, const std::string name, int area) const {
  const int calo = CaloCellCode::CaloNumFromName(name);
  if(calo < 0){
    error() << "Calo name : " << name << "is unknown " <<endmsg;
    return;
  }
  bookCaloFuture2D(unit, title, calo, area);
  return; 
}

//==============================================================================

void CaloFuture2Dview::bookCaloFuture2D(const HistoID& unit, const std::string title, unsigned int calo, int area) const {

  const auto& cp = m_caloParams[calo];
  if( cp.calo == nullptr ){
    Warning("Cannot book calo2D histo "+title+"'", StatusCode::SUCCESS).ignore();
    return;    
  }

  // Pre-booking
  m_caloViewMap[unit] = calo;

  // 1D view : caloCellID as xAxis
  if( m_1d ){
    if( m_profile )
      GaudiHistoAlg::bookProfile1D(unit, title, 0., (double) m_bin1d, m_bin1d, m_prof);
    else
      GaudiHistoAlg::book1D(unit, title, 0., (double) m_bin1d, m_bin1d);
    return;
  }

  // electronics 2D view : FEB vs Channel (include PIN-diode channels)
  if( !m_geo ){
    // Book histo
    TH2* th2 = nullptr;
    if( m_profile ){
      const auto p2d = GaudiHistoAlg::bookProfile2D( unit, title,
        (double) cp.fCard, (double) cp.lCard, cp.lCard-cp.fCard, 
        0., (double) cp.nChan, cp.nChan );
      th2 = Gaudi::Utils::Aida2ROOT::aida2root( p2d );
    } else {
      const auto h2d = GaudiHistoAlg::book2D( unit, title, 
        (double) cp.fCard, (double) cp.lCard, cp.lCard-cp.fCard, 
        0., (double) cp.nChan, cp.nChan );
      th2 = Gaudi::Utils::Aida2ROOT::aida2root( h2d );
    }
    // set Xaxis bin labels
    th2->GetXaxis()->LabelsOption( m_lab.c_str() );
    for( int bin=1; bin <= cp.lCard-cp.fCard; ++bin){
      if( (bin-1)%8 != 0 ) continue;
      int code  = cp.fCard + bin - 1;
      int crate = code / 16;
      int feb   = code - crate * 16;
      std::stringstream loc("");
      loc << "c" << format("%02i",crate) << "f" << format("%02i",feb);
      th2->GetXaxis()->SetBinLabel( bin, loc.str().c_str() );
    }
    return;
  }

  // 2D geometrical view with small bining to simulate variable bin size (separated PMT/PIN histo)
  int fArea = 0;
  int lArea = m_split ? (6+cp.reg)/4 : 1;
  if(m_split && area >= 0 ){
    fArea = area;
    lArea = area+1;
  }
  int ppcm = m_split ? 1 : cp.reg ;
  for(int i = fArea ; i < lArea ; ++i){
    double xmin = cp.refCell[i].col();
    double xmax = (double) cp.centre * 2. - xmin;
    double ymin = cp.refCell[i].row();
    double ymax = (double) cp.centre * 2. - ymin;
    int xbin = (cp.centre-cp.refCell[i].col() )*2 * ppcm; 
    int ybin = (cp.centre-cp.refCell[i].row() )*2 * ppcm;
    if(m_dim){
      xmax = cp.xsize[i] * (xmax-xmin) / 2.;
      xmin = -xmax;
      ymax = cp.ysize[i] * (ymax-ymin) / 2.;
      ymin = -ymax;
    }
    const auto lun  = getUnit(unit, calo, i);
    const auto tl   = getTitle(title, calo, i);
    if(!m_profile) 
      GaudiHistoAlg::book2D( lun, tl, xmin, xmax, xbin, ymin, ymax, ybin);  
    else 
      GaudiHistoAlg::bookProfile2D( lun, tl, xmin, xmax, xbin, ymin, ymax, ybin);
  } 
  return;
}

//==============================================================================

const GaudiHistoAlg::HistoID CaloFuture2Dview::getUnit(const HistoID& unit, int calo, int area) const {
  if(!m_split) return unit;
  const std::string& nArea = CaloCellCode::caloArea( calo, area );
  std::string sunit = unit.literalID();
  if( sunit == "" ) sunit = std::string(unit);
  int index = sunit.find_last_of("/")+1 ;  
  sunit.insert(index, nArea+"/");
  return (HistoID) sunit;
}

//==============================================================================

std::string CaloFuture2Dview::getTitle(std::string title, int calo, int area) const {
  if(!m_split) return title;
  const std::string& nArea = CaloCellCode::caloArea( calo, area );
  std::string tit = title + " (" +nArea + " area)";
  return tit;
}

//==============================================================================

void CaloFuture2Dview::resetTitle(const HistoID& unit, std::string title) const {
  // Abort if invalid unit given
  auto it = m_caloViewMap.find( unit );
  if( it == m_caloViewMap.end())return;

  // Loop over all area
  const int calo = it->second;
  const int lArea = m_split ? (6+m_caloParams[calo].reg)/4 : 1;
  for(int i=0; i<lArea; ++i){
    // first kind
    HistoID lun = (HistoID) getUnit( unit, calo, i );
    std::string tit = getTitle( title, calo, i );
    if( histoExists(lun)) histo2D(lun)->setTitle(tit);
    // second kind
    std::string name = (std::string) unit;
    int index = name.find_last_of("/") +1 ; 
    std::string suff = name.substr( index, name.length() );
    std::string pref = name.substr( 0, index );
    lun =  pref + "PIN/" +  suff ;
    const HistoID& lun2 = getUnit( lun, calo, i );
    if( histoExists((HistoID) lun2 )){
      std::string tit = title + " PIN-diode " ;
      std::string tit2 = getTitle( title, calo, i);
      histo2D( lun2 )->setTitle(tit2);
    }
  }  
}

//==============================================================================

void CaloFuture2Dview::reset(const HistoID& unit, std::string title) const {
  // Abort if invalid unit given
  auto it = m_caloViewMap.find( unit );
  if( it == m_caloViewMap.end())return;

  // Loop over all area
  const int calo = it->second;
  const int lArea = m_split ? (6+m_caloParams[calo].reg)/4 : 1;
  for(int i=0; i<lArea; ++i){
    
    // first kind
// >>>>>>> 0c9c38d... modernise-CaloFuture2Dview
    HistoID lun = (HistoID) getUnit( unit, calo , i);
    if( histoExists(lun) ) histo2D(lun)->reset();
    
    // second kind
    std::string name = (std::string) unit;
    int index = name.find_last_of("/") + 1 ; 
    std::string pref = name.substr( 0, index );
    std::string suff = name.substr( index, name.length() );
    lun = pref + "PIN/" + suff;
    const HistoID& lun2 = getUnit( lun, calo, i );
    if( histoExists( lun2 ) ) histo2D((HistoID) lun2 )->reset();
  } 
  if("" != title) resetTitle(unit, title);
}


//==============================================================================
// FILL CALOFUTURE 2D
//==============================================================================

void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::MCCaloHit mchit, const std::string title) const {
  double weight = m_energyWeighted ? mchit.activeE() : 1.;
  fillCaloFuture2D(unit, mchit.cellID(), weight , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::MCCaloDigit mcdigit, const std::string title) const {
  double weight = m_energyWeighted ? mcdigit.activeE() : 1.;
  fillCaloFuture2D(unit, mcdigit.cellID(), weight , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::CaloDigit digit, const std::string title) const {
  double weight = m_energyWeighted ? digit.e() : 1.;
  fillCaloFuture2D(unit, digit.cellID(), weight  , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::CaloAdc adc, const std::string title) const {
  double weight = m_energyWeighted ? (double) adc.adc() : 1.;
  fillCaloFuture2D(unit, adc.cellID(), weight , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::L0CaloAdc l0adc, const std::string title) const {
  double weight = m_energyWeighted ? (double) l0adc.adc() : 1.;
  fillCaloFuture2D(unit, l0adc.cellID(), weight , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::L0PrsSpdHit hit, const std::string title) const {
  fillCaloFuture2D(unit, hit.cellID(), 1. , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::L0CaloCandidate l0calo, const std::string title) const {
  setL0ClusterView(true);
  double weight = m_energyWeighted ? l0calo.et() : 1.;
  fillCaloFuture2D(unit, l0calo.id(), weight , title);
}
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, LHCb::CaloCluster cluster, const std::string title) const {
  setL0ClusterView(true);
  double weight = m_energyWeighted ? cluster.e() : 1.;
  fillCaloFuture2D(unit, cluster.seed(), weight , title);
}

// The main implementation of fillCaloFuture2D
void CaloFuture2Dview::fillCaloFuture2D(const HistoID& unit, const LHCb::CaloCellID& id, double value, const std::string title) const {
  // threshold
  if( value < m_threshold ) return;
  // separate treatment for PIN-diode channels for 2D geometrical view
  if( m_geo && id.isPin() ){
    if( m_pin ) fillCaloFuturePin2D(unit, id, value, title);
  }
  
  // get calo number
  const unsigned int calo = id.calo();
  const unsigned int area = id.area();
  
  // book histo if not found
  const HistoID& lun = getUnit( unit, calo, area );
  if( !histoExists( lun ) ) bookCaloFuture2D(unit, title, calo, area);

  // get calo parameters
  const auto& cp = m_caloParams[calo];
  const auto* detPtr = cp.calo;
  if( detPtr == nullptr ) return;
  if( ! detPtr->valid(id) )return;


  // check the cellID is consistent with the calo
  if(m_caloViewMap[ unit ] != calo){
    if( msgLevel(MSG::DEBUG) ) 
      debug() << "Cannot put the  CaloCellID " << id << " in the "
              << CaloCellCode::CaloNameFromNum( m_caloViewMap[unit] ) 
              << " view '" << unit << "'" << endmsg;
    return; 
  } 

  // -------------- 1D view
  if( m_1d ){
    if( m_profile ) profile1D(lun)->fill( (double) id.index(), value, 1. );
    else histo1D(lun)->fill( (double) id.index(), value);
    return;
  }

  // -------------- Electronics 2D view (crate/feb .vs. channel)
  if( !m_geo ){
    const int feb   = detPtr->cardNumber( id );
    const int code  = detPtr->cardCode( feb );
    const int chan  = detPtr->cardColumn( id ) + nColCaloCard * detPtr->cardRow( id );
    const double weight = (value + m_offset) / ( m_flux ? cp.xsize[area]*cp.ysize[area] : 1. );
    if( m_profile ) profile2D(lun)->fill( (double) code, (double) chan, weight, 1.);
    else histo2D(lun)->fill( (double) code, (double) chan, weight);
    return;
  }  

  // -------------- 2D geometrical view with small bining to simulate variable cell sizes
  const double xs = cp.xsize[area];
  const double ys = cp.ysize[area];  
  const double xs0 = cp.xsize[0];
  const double ys0 = cp.ysize[0];  
  const double xResize = m_dim ? (m_split ? xs : xs0) : 1.;
  const double yResize = m_dim ? (m_split ? ys : ys0) : 1.;
  const unsigned int ibox = m_split ? 1 : cp.reg/(area+1);
  const int ppcm = m_split ? 1 : cp.reg ;
  const double weight = (value + m_offset) / ( m_flux ? xs*ys : 1. );

  // PMT channels
  const unsigned int row = id.row();
  const unsigned int col = id.col();

  // L0-cluster mode (2x2 cells area are filled)
  const unsigned int icl = m_l0 ? 2 : 1;

  // loop over cluster area (1x1 or 2x2 cells depending on L0 mode)
  for(unsigned int kr = 0 ; kr < icl ; kr++){
    for(unsigned int kc = 0 ; kc < icl ; kc++){
      LHCb::CaloCellID temp = id;
      if( m_l0 ){
        temp = LHCb::CaloCellID(calo, area , row + kr ,  col+ kc);
        if( !detPtr->valid(temp) ) continue;
      }      
      unsigned int theRow = row+kr;
      unsigned int theCol = col+kc;
      // Fill the standard view  for PMT
      // loop over cell area ( ibox * ibox bins depending on CaloFuture/Area)
      for(unsigned int ir = 0 ; ir < ibox ; ir++){
        for(unsigned int ic = 0 ; ic < ibox ; ic++){
          int iic = (theCol-cp.centre )*ibox + ic ;
          int iir = (theRow-cp.centre )*ibox + ir ;
          if(!m_dim){
            iic += ppcm * cp.centre;
            iir += ppcm * cp.centre;
          }
          double x = xResize * ((double) iic+0.5 ) / (double) ppcm ;
          double y = yResize * ((double) iir+0.5 ) / (double) ppcm ;
          if( m_profile ) profile2D(lun)->fill( x, y, weight, 1 );
          else histo2D(lun)->fill( x, y, weight );
        }
      }
    }
  }
  return;
}

//==============================================================================

void CaloFuture2Dview::fillCaloFuturePin2D(const HistoID& unit, const LHCb::CaloCellID& id, double value, const std::string title) const{
  // Validation
  const auto& cp = m_caloParams[id.calo()];
  const auto detPtr = cp.calo;
  if( detPtr == nullptr ) return;
  if( ! detPtr->valid(id) )return;
  if( !id.isPin() ) return;
  if( value < m_threshold ) return;  

  // re-set title and unit for PIN  
  std::string name = (std::string) unit;
  int index = name.find_last_of("/") +1 ; 
  std::string pref = name.substr( 0,index  );
  std::string suff = name.substr( index, name.length()  );
  std::string lun =  pref + "PIN/" +  suff ;

  // Fill the standard view for PIN-diode 
  // -> fill the PMT area covered by the PIN-diode with the PIN ADC value
  HistoID lun2;
  const auto& cells = detPtr->pinChannels( id );
  for( const auto& cell: cells ){
    const unsigned int row  = cell.row();
    const unsigned int col  = cell.col();
    const unsigned int area = cell.area();
    // split area naming :
    lun2 = getUnit(lun, id.calo(), area);
    // book histo if not found 
    if( !histoExists( lun2 )){
      std::string tit = title + " PIN-diode " ;
      bookCaloFuture2D( (HistoID) lun, getTitle(tit, id.calo(), area ), id.calo(), area);
    } 
    // check the cellID is consistent with the calo
    if(m_caloViewMap[(HistoID) lun ] != id.calo() ){
      if( msgLevel(MSG::DEBUG) ) 
        debug() << "Cannot put the CaloCellID " << id << " in the "
                << CaloCellCode::CaloNameFromNum( m_caloViewMap[(HistoID) lun ] ) 
                << " view " << unit << endmsg;
      return;
    }

    // Filling
    const auto xResize  = m_dim ? (m_split ? cp.xsize[area] : cp.xsize[0]) : 1.;
    const auto yResize  = m_dim ? (m_split ? cp.ysize[area] : cp.ysize[0]) : 1.;
    const auto weight   = (value + m_offset) / (m_flux ? (cp.xsize[area]*cp.ysize[area]) : 1.);
    const auto ibox     = m_split ? 1 : cp.reg/(area+1); 
    const auto ppcm     = m_split ? 1 : cp.reg ;

    for(unsigned int ir = 0 ; ir < ibox ; ir++){
      for(unsigned int ic = 0 ; ic < ibox ; ic++){
        int iic = (col-cp.centre )*ibox + ic;
        int iir = (row-cp.centre )*ibox + ir;
        if(!m_dim){
          iic += ppcm * cp.centre;
          iir += ppcm * cp.centre;
        }
        double x = (xResize * ((double) iic +0.5) )/(double) ppcm ;
        double y = (yResize * ((double) iir +0.5) )/(double) ppcm ;
        GaudiHistoAlg::fill( histo2D(lun2), x, y, weight );
      }
    }
  }
  return;
}

