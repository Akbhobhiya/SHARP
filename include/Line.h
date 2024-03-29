#ifndef SHARP_LINE_H
#define SHARP_LINE_H

#include <climits>
#include <opencv2/opencv.hpp>
#include "../external/easyloggingpp_v9.94.2/easylogging++.h"
namespace aapp {

/**
 * A convenient class to represent a point in a two-dimensional space.
 */
class Point : public el::Loggable {
public:
  /**
   * Default constructor. Instantiate a Point centered in (0, 0).
   */
  Point() : _x{0}, _y{0} {}
  /**
   * Instantiate a Point centered in (x, y)
   * @param [in] x the value of X dimension
   * @param [in] y the value of Y dimension
   */
  Point(int x, int y) : _x(x), _y(y) {}

  /**
   * EasyLogging++ method to log a Point.
   */
  void log(el::base::type::ostream_t &os) const {
    os << "(" << _x << ", " << _y << ")";
  }

  /**
   * Equality operator overloading.
   * @param [in] op Point which to compare *this* Point to.
   * @return True if *this* Point and \p op have same _x_ and _y_ coordinates.
   */
  bool operator==(const Point& op) const {
    return _x == op._x && _y == op._y;
  }

  int _x; ///< Position on the X axis
  int _y; ///< Position on the Y axis
};

/**
 * A class representing a segment line in a two dimensional space.
 * A line segment is abstracted by its starting and ending point, its slope and
 * its length.
 * From the analytical geometry we know that the slope of a line varies from
 * [-inf, +inf]. For our purposes a vertical line is set to have a slope
 * equal to DBL_MAX. That's our infinite.
 *
 * We consider as a valid Line also a single Point. A single Point is a
 * degenerate Line, whose length is null.
 */
class Line {
public:
  /**
   * Instantiates a degenerate Line out of \p point. The new line has a null
   * length, while its slope is undefined.
   * @param [in] point Starting point of Line segment.
   */
  explicit Line(Point point)
      : _start(point), _end{0, 0}, _length{0.0}, _slope{0.0}, _degenerate{true} {}

  /**
   * Appends or prepends Point \p p to this Line segment. Point \p p must be
   * adjacent this Line (refer to Line::isAdjacent for a definition of
   * adjacent). If this was a degenerate Line, start and end points are set
   * and the segment line's slope and length are computed. If this was
   * already a proper Line only the length is updated.
   * @param [in] p Point to prepend or append to this Line.
   */
  void addPoint(Point p);

  /**
   * @return the starting Point of this Line
   */
  const Point &getStart() const;

  /**
   * @return the ending Point of this Line
   */
  const Point &getEnd() const;

  /**
   * @return the length of this Line segment.
   */
  double length() const;

  /**
   * @return true if this Line segment is degenerate i.e. is made by only one
   * Point.
   */
  bool isDegenerate() const;

  /**
   * Tests whether \p p is adjacent to this Line.
   * To define adjacency, it's important to understand the underlying space.
   * Our two-dimensional space is discrete and represented as a matrix whose
   * cells represent a (x, y) point. Two adjacent points on the same
   * horizontal line have same Y value and X values whose distance is 1.
   * Generalizing this consideration, a point is adjacent to a line segment
   * if lies on the same line and its X value is (start.X - 1) or (end.X + 1).
   *
   * If this is a degenerate line, every point "right around" the Line's one
   * is adjacent. "Right around" is analytically expressed by the distance
   * between the two points being less than sqrt(2).
   * @param [in] p Point to test if it's adjacent to this Line.
   * @return true if \p p is adjacent. False otherwise.
   */
  bool isAdjacient(Point p) const;

  /**
   * Equality operator overloading.
   * @param [in] ol Line which to compare *this* Line to.
   * @return True if *this* Line and \p ol have same _start_ and _end_ points,
   *         same _slope_ and same _length_. False otherwise.
   */
  bool operator==(const Line& ol) const;
private:
  Point _start;
  Point _end;
  double _slope;
  double _length;
  bool _degenerate;
  constexpr static double sqrt2 = std::sqrt(2);

  friend std::ostream& operator<<(std::ostream& os, const aapp::Line& obj);
};

/**
 * Evaluates the euclidean distance between Point \p p and Point \p p.
 * @param [in] p A Point
 * @param [in] q A Point
 * @return euclidean distance between \p p and \p q.
 */
double distance(Point p, Point q);

/**
 * Evaluates the slope of a Line passing through Points \p p and \p q. If p.X
 * equals q.X theoretically the slope would be +inf or -inf. In our case,
 * it is set to +DBL_MAX or -DBL_MAX. That's our representation of infinite.
 * @param [in] p A Point
 * @param [in] q A Point
 * @return slope of a Line passing through \p p and \p q
 */
double slope(Point p, Point q);
}

#endif // SHARP_LINE_H
