#pragma once

/**
 * An abstract interface to any object exhibiting a cover in a shpae of a box
 */
struct IBoxCover {
  /// accessor to "minimal x" value
  virtual double xMin() const = 0;
  /// accessor to "maximal x" value
  virtual double xMax() const = 0;

  /// accessor to "minimal y" value
  virtual double yMin() const = 0;
  /// accessor to "maximal y" value
  virtual double yMax() const = 0;

  /// accessor to "minimal z" value
  virtual double zMin() const = 0;
  /// accessor to "maximal z" value
  virtual double zMax() const = 0;
};

