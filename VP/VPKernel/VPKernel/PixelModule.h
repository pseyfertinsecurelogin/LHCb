#ifndef PIXELMODULE_H
#define PIXELMODULE_H 1

/** @class Pixelmodule Pixelmodule.h
 *  Class to hold one VP module
 *
 *  @author Olivier Callot
 *  @author Sebastien Ponce
 *
 *  moved from REC/Pr/PrKernel/PrKernel to LHCB on 20180710
 */
class PixelModule final {

 public:
  /// Constructor
  PixelModule(const unsigned int number, const bool right)
      : m_number(number),
        m_isRight(right) {}

  unsigned int number() const { return m_number; }
  int previous() const { return m_previous; }
  bool isRight() const { return m_isRight; }
  float z() const { return m_z; }

  void setPrevious(const int prev) { m_previous = prev; }
  void setZ(const float z) { m_z = z; }

 private:
  /// Module number
  unsigned int m_number = 999;
  // Number of neighbouring same-side module towards smaller z
  int m_previous = -1;
  /// Z-position
  float m_z;
  /// Right or left side of VELO
  bool m_isRight = true;
};
#endif  // PIXELMODULE_H
