/**
 * \brief Root class for all the personnages in WG
 * \file Perso.hpp
 */

#ifndef __PERSO__HH__
#define __PERSO__HH__

#include <vector>

#include "WGObject.hpp"
#include "PState.hpp"

/**
 * \brief Root class for all personnages
 * \class Perso
 */
class Perso : public WGObject {
public :
  /**
   * \brief Default constructor
   */
  Perso();
  /**
   * \brief Copy constructor
   */
  Perso(Perso &p);
  /**
   * \brief Constructor
   * \param obj The Qt parent object
   */
  Perso(QObject* obj);
  /**
   * \brief Constructor
   * \param obj The WG object
   */
  Perso(WGObject &obj);
  /**
   * \brief Constructor
   */
  Perso(double HP = 0.0, double MP = 0.0,
	double strength = 0.0, double power = 0.0,
	double def = 0.0, double mr = 0.0, 
	double luck = 0.0, int lvl = 0, int mob = 0, 
	std::vector<PState> state = std::vector<PState>(),
	QObject* obj = 0);
  /**
   * \brief Destructor
   */
  ~Perso();
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
   * \brief Set Mobility
   */
  void
  set_mobility(const int mobility);
  /**
   * \brief Set level
   */
  void
  set_level(const int level);
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
  /**
   * \brief Get mobility
   */
  int
  get_mobility();
  /**
   * \brief Get level
   */
  int
  get_level();
  /**
   * \brief Increment level
   */
  int
  inc_level();
protected :
  /**
   * \brief Level of the char
   */
  int  level;
  /**
   * \brief Mobility of the perso
   */
  int _mobility;
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
  /**
   * \brief
   */
  std::vector<PState> _state;
};
