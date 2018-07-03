#ifndef VPRETINACLUSTER_H
#define VPRETINACLUSTER_H 1


/*
 * @author Federico Lazzari
 * @date   2018-06-22
 */

namespace LHCb
{

  class VPRetinaCluster final
  {
  public:
    /// Constructor
    VPRetinaCluster( const uint32_t word, const float gx, const float gy , const float tan)
      : m_word( word ), m_gx( gx ), m_gy( gy ), m_tan( tan ) {}

    ///
    uint32_t word() const noexcept { return m_word; }

    ///
    float gx() const noexcept { return m_gx; }

    ///
    float gy() const noexcept { return m_gy; }
 
    ///
    float tan() const noexcept { return m_tan; }

  private:
    uint32_t m_word;     /// RetinaCluster value
    float m_gx;          /// RetinaCluster global x coordinate
    float m_gy;          /// RetinaCluster global y coordinate
    float m_tan;         /// RetinaCluster tan
  }; // class VPRetinaCluster

  /// lightweight container for VPRetinaCluster
  typedef std::vector<VPRetinaCluster> VPRetinaClusters;

} // namespace LHCb;


#endif // VPRETINACLUSTER_H

