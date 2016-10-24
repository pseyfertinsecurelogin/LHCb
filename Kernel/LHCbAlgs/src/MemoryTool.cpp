// =============================================================================
// Include files 
// =============================================================================
// from Gaudi
#include "GaudiKernel/Memory.h"
// =============================================================================

// local
#include "MemoryTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MemoryTool
//
// 2005-12-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( MemoryTool )

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
MemoryTool::MemoryTool( const std::string& type,
                        const std::string& name,
                        const IInterface* parent )
  : base_class ( type, name , parent )
{
  declareProperty( "HistoSize"  , m_bins = 500 );
  //
  declareProperty 
    ( "SkipEvents" , 
      m_skip = 10  , 
      "Skip the first N events from delta memory counter " );
  declareProperty 
    ( "TotalMemoryHisto" , 
      m_histo1      , 
      "The parameters of 'total memory' histogram" ) ;
  declareProperty 
    ( "DeltaMemoryHisto" , 
      m_histo2      , 
      "The parameters of 'delta memory' histogram" ) ;
  //
  declareProperty 
    ( "Check"  , 
      m_check = 20 , 
      "Frequency for checks for suspision memory leak" ) ;
  //
  declareProperty 
    ( "MaxPrints" , 
      m_maxPrint = 0 , 
      "Maximal number of print-out" ) ;
  //
  setProperty ( "HistoPrint", false );
  declareInterface<IGenericTool>(this);
}
// ============================================================================
// finalize the tool 
// ============================================================================
StatusCode MemoryTool::initialize () 
{
  auto sc = base_class::initialize (); 
  static const std::string s_TotalMemory = "Total Memory/MB" ;
  static const std::string s_DeltaMemory = "Delta Memory/MB" ;
  if ( !m_totMem                   ) { m_totMem = &counter( s_TotalMemory ) ; }
  if ( !m_delMem                   ) { m_delMem = &counter( s_DeltaMemory ) ; }
  if ( !m_plot1 && produceHistos() ) { m_plot1  = book    ( m_histo1      ) ; }
  if ( !m_plot2 && produceHistos() ) { m_plot2  = book    ( m_histo2      ) ; }
  return sc;
}
// ============================================================================
// finalize the tool 
// ============================================================================
StatusCode MemoryTool::finalize () 
{
  if ( m_delMem                     && 
       1 < m_delMem->nEntries    () && 
       0 < m_delMem->flagMean    () && 
       0 < m_delMem->flagMeanErr () && 
       m_delMem->flagMean() > 3 * m_delMem->flagMeanErr() ) 
  {
    Warning( "Mean 'delta-memory' exceeds 3*sigma", StatusCode::SUCCESS).ignore();
  }
  m_totMem = nullptr ;
  m_delMem = nullptr ;
  m_plot1  = nullptr ;
  m_plot2  = nullptr ;
  return base_class::finalize (); 
}
//=============================================================================
// Plot the memory usage
//=============================================================================
void MemoryTool::execute() /*const*/
{
  const auto lmem = System::virtualMemory();
  const double mem = lmem;

  // get/set "previous" measurement
  auto prev = double(m_prev.exchange(lmem))/1000.;  // memory in MB
  // memory in megabytes 
  const auto memMB = mem / 1000. ;
  /// grab current value, and (post!)increment event counter
  auto counter = m_counter.fetch_add(1);

  // reset the counter (if required ) 
  if  ( 0 < m_skip &&  m_skip == counter && 0 != m_delMem ) 
  {
    // NB: I hate StatEntity::reset, use == instead!
    *m_delMem = StatEntity() ;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug()  << "Reset Delta Virtual Memory counter" << endmsg ;
  }
  // Fill the counter for "valid" previous measurements
  if   ( m_totMem ) { *m_totMem += memMB ; }
  // Fill the plot
  if   ( m_plot1  ) { fill ( m_plot1 , memMB , 1 , m_histo1.title() ) ; }


  // Fill the counter for "valid" previous measurements
  const auto deltaMem = memMB - prev ;
  if   ( 0 <= prev   )
  { 
    // fill the counter 
    if ( m_delMem ) { *m_delMem += deltaMem ; }
    // fill the counter
    if ( m_plot2  ) { fill ( m_plot2 , deltaMem , 1 , m_histo2.title() ) ; }
  }
  
  if ( m_bins > counter ) 
  {
    plot( counter+1, "Virtual mem, all entries", "Virtual memory (kB), first 'HistoSize' entries",
          0.5, m_bins+0.5, m_bins, mem );
  }
  if( 0 == counter%m_bins ) {
    const unsigned int bin = 1 + ( counter/m_bins );
    plot( bin, "Virtual mem, downscaled", "Virtual memory (kB), downscaled entries",
          0.5, m_bins+0.5, m_bins, mem );
  }
  
  
  // check Total Memory for the particular event
  if ( m_totMem                          && 
       16 <  m_totMem->nEntries       () && 
       0  <  m_totMem->flagMean       () && 
       0  <  m_totMem->flagRMS        () && 
       0  <  memMB                       && 
       memMB > m_totMem->flagMean() + 3 * m_totMem->flagRMS () ) 
  {
    Warning ( "Total Memory for the event exceeds 3*sigma" , 
              StatusCode::SUCCESS , m_maxPrint     ).ignore() ;    
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug () << " Total Memory : " << memMB 
               << " Mean : ("        << m_totMem->flagMean () 
               << "+-"               << m_totMem->flagRMS() << ")" << endmsg ;
  }
  // check the particular event
  if ( 0  <= m_prev                      && 
       m_delMem                          && 
       16 <  m_delMem->nEntries       () && 
       0  <  m_delMem->flagRMS        () && 
       0  <  deltaMem                    && 
       deltaMem > m_delMem->flagMean() + 3 * m_delMem->flagRMS () ) 
  {
    Warning ( "Delta Memory for the event exceeds 3*sigma" , 
              StatusCode::SUCCESS , m_maxPrint     ).ignore() ;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug () << " Delta Memory : "  << deltaMem
               << " Mean : ("         << m_delMem->flagMean () 
               << "+-"                << m_delMem->flagRMS() << ")" << endmsg ;
  }
  /// check the tendency: 
  if ( ( ( 0 < m_check && 0 == counter % m_check ) || 1 == m_check  ) && 
       m_delMem                      && 
       16 < m_delMem->nEntries    () && 
       0  < m_delMem->flagMean    () && 
       0  < m_delMem->flagMeanErr () && 
       m_delMem->flagMean() > 3 * m_delMem->flagMeanErr() ) 
  {
    Warning ( "Mean 'Delta-Memory' exceeds 3*sigma" , 
              StatusCode::SUCCESS , m_maxPrint     ).ignore() ;
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug () << " Memory Leak? "
               << "("  << m_delMem->flagMean() 
               << "+-" << m_delMem->flagMeanErr() << ")" << endmsg ;
  }
  
}
// ============================================================================
// The END 
// ============================================================================
