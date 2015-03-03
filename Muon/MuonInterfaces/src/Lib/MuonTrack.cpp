// $Id: MuonTrack.cpp,v 1.2 2010-03-17 10:31:07 ggiacomo Exp $
#define MUONTRACKRECNMSPC
#include "MuonInterfaces/MuonTrack.h"
#include "MuonInterfaces/MuonLogPad.h"

#ifdef _WIN32
    #define DLLEXPORT __declspec(dllexport) 
#else
    #define DLLEXPORT
#endif

namespace MuonTrackRec {
  DLLEXPORT double muspeed = 468.42; // speed of light in mm/TDC units
  DLLEXPORT double Zref = 17670; // default ref is M4
  DLLEXPORT bool PhysTiming = false;
  DLLEXPORT bool IsCosmic = false;
  DLLEXPORT bool IsPhysics = false;
};
#undef MUONTRACKRECNMSPC

//=============================================================================
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonTrack::MuonTrack() : m_isClone(false), m_chi2x(-1.), m_chi2y(-1.), 
                         m_sx(-1.), m_bx(-1.), m_sy(-1.), m_by(-1.),
                         m_errsx(-1.), m_errbx(-1.), m_covbsx(-1.),
                         m_errsy(-1.), m_errby(-1.), m_covbsy(-1.),
                         m_t0(-1.), m_errt0(-1), m_sigmat(-1.), 
                         m_speed(0.), m_sigmaspeed(0.), 
                         m_speedFitted(false), 
                         m_trackP(0.), m_trackPt(0.),
                         m_trackqOverP(0.), m_trackMomentum(0.,0.,0.) {}
//=============================================================================
// Destructor
//=============================================================================
MuonTrack::~MuonTrack() {} 
//=============================================================================

//=============================================================================
void MuonTrack::insert( const int id, const MuonHit* xyz ){
  m_muonTrack.insert( MuonTrack::MuonTkVtype(id, const_cast<MuonHit*>(xyz)) );
}
///
std::vector< MuonHit* > MuonTrack::getHits(){
  std::vector< MuonHit* > hits;
  MuonTrack::MuonTkIt tk = m_muonTrack.begin();
  for(; tk != m_muonTrack.end(); tk++){
    hits.push_back( tk->second );
  }
  return hits;
}
///
void MuonTrack::setBestCandidate( std::vector<MuonHit*> bcand ) 
{
  m_bestcand = bcand;
}
///
std::vector<MuonHit*> MuonTrack::bestCandidate() 
{
  return m_bestcand;
}
/// set the clone flag
void MuonTrack::setClone()
{
  m_isClone = true;
}
/// get the Clone flag
bool MuonTrack::isClone()
{
  return m_isClone;
}
///
int MuonTrack::getSpan(){
  MuonTrack::MuonTkIt iv = m_muonTrack.begin();
  int smax = -9999;
  int smin =  9999;
  for(;iv != m_muonTrack.end(); iv++){
    int s = iv->second->station();
     if(s >= smax) smax = s;
     if(s <= smin) smin = s;
  }
  return (smax-smin);
}
///
int MuonTrack::getFiringStations(){
  MuonTrack::MuonTkIt iv;
  int firstat = 0;
  for(int s=0; s<5; s++){
    for(iv = m_muonTrack.begin() ;iv != m_muonTrack.end(); iv++){
      int hs = iv->second->station();
      if(hs == s){
        firstat++;
        break;
      }
    }
  }
  return firstat;
}

StatusCode MuonTrack::speedFit() {
  // first extract the hits 
  MuonTrack::MuonTkIt tk;
  double dof = m_muonTrack.size() - 2.;
  if(dof<0) return StatusCode::FAILURE;

  double tc11,tc12,tc22,tv1,tv2,ttt;
  tc11 = tc12 = tc22 = tv1 = tv2 = ttt = 0;
  double tdet,t,xp,yp,z,d;

  for(tk = m_muonTrack.begin(); tk != m_muonTrack.end(); tk++){
    MuonHit* hit= tk->second;
    xp = hit->X() - m_bx; 
    yp = hit->Y() - m_by; 
    z  = hit->Z();
    d = sqrt(xp*xp+yp*yp+z*z);
    t = correctedTime(*hit);

    tc11 += d*d;
    tc12 +=   d;
    tc22 += 1.0;
    tv1  += d*t;
    tv2  +=   t;
    ttt  += t*t;

  }
  if( (tdet = tc11*tc22 - tc12*tc12) == 0 ) return StatusCode::FAILURE;
  double invSpeed = (tv1*tc22-tv2*tc12)/tdet;
  double sigma_invSpeed = sqrt(tc22/tdet);
  double t0 = (tv2*tc11-tv1*tc12)/tdet;

  double sumResq = (ttt + invSpeed*invSpeed*tc11 + 
                    t0*t0*tc22 - 2.*invSpeed*tv1 -2.*t0*tv2 + 2*invSpeed*t0*tc12);
  sigma_invSpeed *= sqrt(sumResq/dof);
  m_speed = 1/invSpeed;
  m_sigmaspeed = m_speed*m_speed *sigma_invSpeed;
  // convert to c units
  m_speed /= MuonTrackRec::muspeed;
  m_sigmaspeed /= MuonTrackRec::muspeed;
  m_speedFitted = true;
  return StatusCode::SUCCESS;
}

double MuonTrack::correctTOF(double rawT,
                             double X,
                             double Y,
                             double Z) {
 return ( rawT +
           (sqrt(X*X+Y*Y+Z*Z)-MuonTrackRec::Zref)/MuonTrackRec::muspeed);
}


double MuonTrack::correctTime(double rawT,
                              double X,
                              double Y,
                              double Z) {
  double t = rawT;
  if (MuonTrackRec::PhysTiming) { // correct for TOF applied in delays (from primary vertex)
    t = correctTOF(t, X, Y, Z);
  }
  return t;
}

double MuonTrack::correctedTime(MuonHit& hit) {
  double tc = hit.hitTime();
  return correctTime( tc, hit.X(), hit.Y(), hit.Z());
}

/// track fitting with linear Chi^2
StatusCode MuonTrack::linFit()
{
  // first extract the hits 
  MuonTkIt tk;
  double dof = m_muonTrack.size() - 2.;
  if(dof<0) return StatusCode::FAILURE;

  double xc11,xc12,xc22,xv1,xv2,xxx;
  xc11 = xc12 = xc22 = xv1 = xv2 = xxx = 0;
  double yc11,yc12,yc22,yv1,yv2,yyy;
  yc11 = yc12 = yc22 = yv1 = yv2 = yyy = 0;
  double xdet,ydet;
  double  xerr,yerr,x,y,z;

  for(tk = m_muonTrack.begin(); tk != m_muonTrack.end(); tk++){
    MuonHit* hit= tk->second;
    xerr = 2.* hit->dX();
    yerr = 2.* hit->dY();

    x = hit->X(); 
    y = hit->Y(); 
    z = hit->Z();

    // then fit them with a min chi^2 in the 2 projections xz and yz
    //1) XZ projection:
    xc11 += z*z/xerr/xerr;
    xc12 +=   z/xerr/xerr;
    xc22 += 1.0/xerr/xerr;
    xv1  += z*x/xerr/xerr;
    xv2  +=   x/xerr/xerr;
    xxx  += x*x/xerr/xerr;
    
    //2) YZ projection:
    yc11 += z*z/yerr/yerr;
    yc12 +=   z/yerr/yerr;
    yc22 += 1.0/yerr/yerr;
    yv1  += z*y/yerr/yerr;
    yv2  +=   y/yerr/yerr;
    yyy  += y*y/yerr/yerr;

  }
  if( (xdet = xc11*xc22 - xc12*xc12) == 0 ) return StatusCode::FAILURE;
  if( (ydet = yc11*yc22 - yc12*yc12) == 0 ) return StatusCode::FAILURE;
  m_sx = (xv1*xc22-xv2*xc12)/xdet;
  m_sy = (yv1*yc22-yv2*yc12)/ydet;
  m_bx = (xv2*xc11-xv1*xc12)/xdet;
  m_by = (yv2*yc11-yv1*yc12)/ydet;
  
  m_errbx = sqrt(xc11/xdet);
  m_errsx = sqrt(xc22/xdet);
  m_covbsx = -xc12/xdet;

  m_errby = sqrt(yc11/ydet);
  m_errsy = sqrt(yc22/ydet);
  m_covbsy = -yc12/ydet;
  
  m_chi2x = (xxx + m_sx*m_sx*xc11 + m_bx*m_bx*xc22 - 2.*m_sx*xv1 -2.*m_bx*xv2 + 2*m_sx*m_bx*xc12)/dof;
  m_chi2y = (yyy + m_sy*m_sy*yc11 + m_by*m_by*yc22 - 2.*m_sy*yv1 -2.*m_by*yv2 + 2*m_sy*m_by*yc12)/dof;
  


  //3) fit track time (assuming constant error on time and neglecting error on distance of flight)

  if (false == MuonTrackRec::IsCosmic && false == MuonTrackRec::IsPhysics ) {
    // get particle direction from sign of fitted speed
    // (for cosmics, we can use the y slope)
    speedFit();
  }

  double xp,yp,d,t;
  double sumt,sumd,sumt2,sumd2,sumtd;
  sumt = sumd = sumt2 = sumd2 = sumtd = 0.;

  for(tk = m_muonTrack.begin(); tk != m_muonTrack.end(); tk++){
    MuonHit* hit= tk->second;
    xp = hit->X() - m_bx; 
    yp = hit->Y() - m_by; 
    z  = hit->Z();
    d = sqrt(xp*xp+yp*yp+z*z)/MuonTrackRec::muspeed;
    t = correctedTime(*hit);
    sumt  += t   ; sumd  += d;
    sumt2 += t*t ; sumd2 += d*d;
    sumtd += t*d ;
  }
  // compute t0 choosing track direction according to y slope
  m_t0 = ( sumt - sz()*sumd )/m_muonTrack.size();
  m_sigmat = sqrt( (sumt2 + sumd2 - sz()*2*sumtd - m_muonTrack.size()*m_t0*m_t0 ) / (m_muonTrack.size()-1) );
  m_errt0 = m_sigmat/sqrt(float(m_muonTrack.size()));
  return StatusCode::SUCCESS;
}

void MuonTrack::extrap(double Z, 
                       Gaudi::XYZTPoint& Pos, 
                       Gaudi::XYZTPoint* PosErr) {

  double dX= m_sx * Z, dY= m_sy * Z;
  Pos.SetCoordinates( m_bx + dX,
                      m_by + dY,
                      Z,
                      m_t0 + sz() * sqrt(dX*dX+dY*dY+Z*Z)/MuonTrackRec::muspeed);
  if(PosErr)
    PosErr->SetCoordinates( sqrt(m_errbx*m_errbx + Z*Z*m_errsx*m_errsx + 2*Z*m_covbsx),
                            sqrt(m_errby*m_errby + Z*Z*m_errsy*m_errsy + 2*Z*m_covbsy),
                            0.,
                            m_errt0);
}

Gaudi::XYZTPoint& MuonTrack::residuals(MuonHit& hit) {
  extrap(hit.Z(), m_point);
  double t = correctedTime(hit);
  m_point.SetCoordinates( hit.X() - m_point.X(),
                          hit.Y() - m_point.Y(),
                          0.,
                          t - m_point.T() );
  return m_point;
}

// Z versor giving the track direction (obtained from y slope for cosmics)
double MuonTrack::sz() const {return ( MuonTrackRec::IsPhysics ? 1. : 
                                     ( MuonTrackRec::IsCosmic ? (m_sy>0 ? -1.: 1.):
                                       (m_speed>0 ? 1.: -1.) ) );}

// attach Xtalk hits (by Silvia Pozzi)
StatusCode MuonTrack::AddXTalk(const std::vector< MuonHit* >* trackhits, float cut)
{
  int nXTalk = 0;
  
  StatusCode sc = linFit();
  if(!sc) 
    return StatusCode::FAILURE;
  
  // first extract the hits 
  MuonTkIt tk;
  std::vector< MuonHit* > tt_hits ;

  std::vector<MuonHit*>::const_iterator ihT,ihH,ihK; 
  for (ihT = trackhits->begin(); ihT != trackhits->end() ; ihT++){ // loop on all the hits
    
    for (tk = m_muonTrack.begin(); tk != m_muonTrack.end(); tk++){ // loop on the track hits
      MuonHit* hit= tk->second;
      if ( hit->station() == (*ihT)->station() && 
           hit->hitID()   != (*ihT)->hitID() ) {
        
        double deltaX;
        double deltaY;
          
        deltaX = fabs( (*ihT)->X() - ( m_bx + m_sx *(*ihT)->Z() ) ) / ((*ihT)->hitTile_dX()) ;
        deltaY = fabs( (*ihT)->Y() - ( m_by + m_sy *(*ihT)->Z() ) ) / ((*ihT)->hitTile_dY()) ;

        if (deltaX<cut && deltaY<cut ) {
          bool trovato = true;
          for (ihK=tt_hits.begin(); ihK!=tt_hits.end(); ihK++) 
            if ((*ihT)==(*ihK) ) trovato = false;
          if (trovato){
            tt_hits.push_back(*ihT);  
            nXTalk++;
          }  
        } //if (deltaX < ... && deltaY<...)
      } // if hit->... == (*ihT)...
      
    } // loop on m_trackhits
  } // loop on t_hits
  
  int idhit=100;
  std::vector<MuonHit* >::iterator tj;
  for (tj=tt_hits.begin(); tj!=tt_hits.end(); tj++ ){
    idhit++;
    this->insert(idhit,( *tj) );
  }

  return StatusCode::SUCCESS;
}

int MuonTrack::clsize(MuonHit* hit, int& xsize, int& ysize) {
  // cluster size is computed only for the first hit of a given station, return -1 otherwise
  std::vector< MuonHit* > hits=getHits();
  bool start=false;
  xsize=ysize=0;
  int np=0;
  MuonHit *tkh, *prevh;
  
  for (std::vector< MuonHit* >::iterator itkh=hits.begin() ; itkh != hits.end(); itkh++) {
    tkh=*itkh;
    if (!start && tkh != hit && tkh->station() == hit->station()) {
      xsize=ysize=-1;
      return -1;
    }
    if (tkh == hit) start=true;
    if (start && tkh->station() == hit->station()) {
      bool prendilax=true,prendilay=true;
      // check that this position is not already the same of a previous pad      
      for (std::vector< MuonHit* >::iterator iprevh=hits.begin() ; iprevh<itkh ; iprevh++) {
        prevh=*iprevh;
        if(prevh->station() == tkh->station()) {
          if ( fabs(tkh->X() - prevh->X())< tkh->hitTile_dX()) { 
            prendilax=false;
          }
          if ( fabs(tkh->Y() - prevh->Y())< tkh->hitTile_dY() ) { 
            prendilay=false;
          }
        }
      }
      np += tkh->npads();
      if (prendilax) {
        xsize += tkh->clsizeX();
      }
      if (prendilay) {
        ysize += tkh->clsizeY();
      }
    }
  }
  return np;
}
