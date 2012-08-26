/**
 * \brief Root class for all the objects in WG
 * \file WGObject.hh
 */

#ifndef __WGOBJECT__HH__
#define __WGOBJECT__HH__

#include <QObject>

#include "Position.hpp"
/**
 * \brief Root class for all objects
 * \class WGObject
 */
class WGObject : public QObject {
public :
  /**
   * \brief Default constructor
   */
  WGObject();
  /**
   * \brief Constructor
   * \param obj The Qt parent object
   */
  WGObject(QObject* obj);
  /**
   * \brief Copy constructor
   */
  WGObject(WGObject &obj);
  /**
   * \brief Destructor
   */
  ~WGObject();
  /**
   * \brief Set the name
   * \param name The new name
   */
  void
  setName(const std::string &name);
  /**
   * \brief Set the x position value
   * \param x The new xvalue
   */
  void
  setPosition(const Position& p);
  /**
   * \brief Get the name 
   * \return The name
   */
  std::string
  getName();
  /**
   * \brief Get the position
   * \return The position
   */
  Position&
  getPosition();
  /**
   * \brief Move the object
   * \param pos The shift for the position
   */
  void
  move(Position &pos);
protected:
  /**
   * \brief Name of the object
   */
  std::string _name;
  /**
   * \brief x position of the object
   */
  Position _pos;
};

#endif // __WGOBJECT__HH__
