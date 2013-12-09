/**
 * \brief The 3D position (x,y,z)
 * \file Position.hh
 */

#ifndef __POSITION__HH__
#define __POSITION__HH__

class QPoint;
/**
 * \brief A 3D position
 * \class Position
 */
class Position {
public :
  /**
   * \brief  Default constructor
   */
  Position();
  /**
   * \brief  Copy constructor
   */
  Position(Position& p);
  /**
   * \brief Constructor
   */
  Position(const double &x, const double& y, const double& z);
  /**
   * \brief Destructor
   */
  ~Position();
  /**
   * \brief Get the x value
   * \return The x value
   */
  double
  getX();
  /**
   * \brief Get the y value 
   * \return The y value
   */
  double
  getY();
  /**
   * \brief Get the z value 
   * \return The z value
   */
  double
  getZ();
  /**
   * \brief Set the x position value
   * \param x The new xvalue
   */
  void
  setX(const double& x);
  /**
   * \brief Set the y position value
   * \param y The new yvalue
   */
  void
  setY(const double& y);
  /**
   * \brief Set the z position value
   * \param z The new xvalue
   */
  void
  setZ(const double& z);
  /**
   * \brief Set the current position
   * \param p The net position
   */
  void
  SetPosition(Position& p);
  /**
   * \brief Update the current position moving of p
   * \param p The shift position
   */
  void
  move(Position& p);

  /**
   * @brief distance_to
   * @param pt
   * @return the distance between this point and pt
   */
  int
  distance_to(const QPoint &pt);

private:
  /**
   * \brief x position of the object
   */
  double _x;
  /**
   * \brief y position of the object
   */
  double _y;
  /**
   * \brief x position of the object
   */
  double _z;

};

#endif // __POSITION__HH__
