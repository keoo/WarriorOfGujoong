/**
 * \brief Root class for all the personnages in WG
 * \file Perso.hpp
 */

#ifndef __PERSO__HH__
#define __PERSO__HH__

#include <vector>

#include "WGObject.hpp"
#include "PState.hpp"
#include "Item.hpp"

/**
 * \brief Root class for all personnages
 * \class Perso
 */
class Perso : public WGObject {
    Q_OBJECT

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
  Perso(std::string name,
        double HP = 0.0, double MP = 0.0,
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
  /**
   * \brief Get the shield
   */
  Item&
  get_shield();
  /**
   * \brief Set the shield
   */
  void
  set_shield(Item& shield);
  /**
   * \brief Get the weapon
   */
  Item&
  get_weapon();
  /**
   * \brief Set the weapon
   */
  void
  set_weapon(Item& weapon);
  /**
   * \brief Get current XP
   */
  int
  get_XP();
  /**
   * \brief Get total XP
   */
  int
  get_max_XP();
  /**
   * \brief Set current XP
   */
  void
  set_XP(int val);
  /**
   * \brief Set total XP
   */
  void
  set_max_XP(int val);
  /**
   * \brief Increase current XP
   */
  void
  inc_XP(int val);
  /**
   * \brief Increase total XP
   */
  void
  inc_max_XP(int val);
  /**
   * \brief Get the states of the perso
   */
  std::vector<PState>
  get_states();
  /**
   * \brief Add a state to the perso
   */
  void
  add_state(PState state);
  /**
   * \brief Remove a state to the perso
   */
  void
  remove_state(PState state);
  /**
   * \brief Remove all bad state to the perso
   */
  void
  clear_states();

  /**
   * \brief Return true if the perso has moved for this turn
   */
  bool has_moved() const;

protected :
  /**
   * \brief Level of the char
   */
  int  _lvl;
  /**
   * \brief Mobility of the perso
   */
  int _mob;
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
   * \brief The states of the perso
   */
  std::vector<PState> _states;
  /**
   * \brief The shield
   */
  Item _shield;
  /**
   * \brief The weapon
   */
  Item _weapon;
  /**
   * \brief Current XP
   */
  int _xp;
  /**
   * \brief Total XP
   */
  int _max_xp;

  /**
   * \brief if the perso has moved for this turn
   */
  bool _has_moved;

public slots:
  /**
   * \brief slot_set_has_moved
   * Called when the animation of the moving perso is finished
   */
  void slot_set_has_moved(bool);
};

#endif
