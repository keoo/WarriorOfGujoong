/**
 * \brief Root class for all the items in WG
 * \file Item.hpp
 */

#ifndef __ITEM__HH__
#define __ITEM__HH__

#include <QObject>

#include "WGObject.hpp"
#include "PState.hpp"

/**
 * \brief Root class for all personnages
 * \class Perso
 */
class Item : public WGObject {
public :
  /**
   * \brief Default constructor
   */
  Item();
  Item();
  /**
   * \brief Copy constructor
   */
  Item(Item &it);
  /**
   * \brief Constructor
   * \param obj The Qt parent object
   */
  Item(WGObject &obj);
  /**
   * \brief Constructor
   * \param obj The Qt parent object
   */
  Item(double HP = 0.0, double MP = 0.0,
       double strength = 0.0, double power = 0.0,
       double def = 0.0, double mr = 0.0, 
       double luck = 0.0,
       QObject* obj = 0);
  ~Item();

  /**
   * \brief Set HP
   */
  void
  set_HP(const double hp);
  /**
   * \brief Set MP
   */
  void
  set_MP(const double mp);
  /**
   * \brief Set defense
   */
  void
  set_def(const double def);
  /**
   * \brief Set phiscal attack
   */
  void
  set_strength(const double ad);
  /**
   * \brief Set power
   */
  void
  set_power(const double power);
  /**
   * \brief Set magic resistance
   */
  void
  set_MR(const double mr);
  /**
   * \brief Set luck
   */
  void
  set_luck(const double luck);
  /**
   * \brief Get HP
   */
  double
  get_HP();
  /**
   * \brief Get MP
   */
  double
  get_MP();
  /**
   * \brief Get defense
   */
  double
  get_def();
  /**
   * \brief Get attack
   */
  double
  get_strength();
  /**
   * \brief Get power 
   */
  double
  get_power();
  /**
   * \brief Get magic resist
   */
  double
  get_MR();
  /**
   * \brief Get luck
   */
  double
  get_luck();

protected :
  /**
   * \brief Heal of the perso
   */
  double _HP;
  /**
   * \brief Mana to launch spells
   */
  double _MP;
  /**
   * \brief Physical damage
   */
  double _strength;
  /**
   * \brief Magic power
   */
  double _power;
  /**
   * \brief Physical defense
   */
  double _def;
  /**
   * \brief Magic resistance
   */
  double _mr;
  /**
   * \brief Luck for critical strikes
   */
  double _luck;
};
