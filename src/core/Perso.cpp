/**
 * \brief Implementation of the root class for all the personnages in WG
 * \file Perso.cpp
 */
#include <vector>
/* -- */
#include <QString>
#include <QFile>
#include <QDomElement>
/* -- */
#include "Item.hpp"
#include "Perso.hpp"

Perso::Perso():WGObject(NULL),_shield(), _weapon(){
    _name = "";
    _HP = 0.0;
    _MP = 0.0;
    _strength = 0.0;
    _power = 0.0;
    _def = 0.0;
    _mr = 0.0;
    _luck = 0.0;
    _lvl = 0;
    _mob = 0;
    _states = std::vector<PState>();
    set_moved(false);
}

Perso::Perso(Perso &p):WGObject(p.parent()){
    _name = p.get_name();
    _HP = p.get_HP();
    _MP = p.get_MP();
    _strength = p.get_strength();
    _power = p.get_power();
    _def = p.get_def();
    _mr = p.get_MR();
    _luck = p.get_luck();
    _states = p.get_states();
    set_moved(false);
}

Perso::Perso(QObject* obj):WGObject(obj){
    _name = "";
    _HP = 0.0;
    _MP = 0.0;
    _strength = 0.0;
    _power = 0.0;
    _def = 0.0;
    _mr = 0.0;
    _luck = 0.0;
    _lvl = 0;
    _mob = 0;
    _states = std::vector<PState>();
    set_moved(false);
}

Perso::Perso(WGObject &obj):WGObject(&obj){
    _HP = 0.0;
    _MP = 0.0;
    _strength = 0.0;
    _power = 0.0;
    _def = 0.0;
    _mr = 0.0;
    _luck = 0.0;
    _lvl = 0;
    _mob = 0;
    _states = std::vector<PState>();
}


Perso::Perso(std::string name, int perso_id,
             double HP, double MP,
             double strength, double power,
             double def, double mr,
             double luck, int lvl, int mob,
             std::vector<PState> state,
             QObject* obj):WGObject(obj) {
    _name = name;
    _player_id = perso_id;
    _HP = HP;
    _MP = MP;
    _strength = strength;
    _power = power;
    _def = def;
    _mr = mr;
    _luck = luck;
    _lvl = lvl;
    _mob = mob;
    _states = state;
}

Perso::~Perso(){
}

void
Perso::set_HP(const double hp){
    _HP = hp;
    if(hp <= 0) {
        emit signal_perso_is_dead(this);
    }
}

void
Perso::set_max_HP(const double hp){
    _max_HP = hp;
}

void
Perso::set_MP(const double mp){
    _MP = mp;
}

void
Perso::set_max_MP(const double mp){
    _max_MP = mp;
}

void
Perso::set_def(const double def){
    _def = def;
}

void
Perso::set_strength(const double ad){
    _strength = ad;
}

void
Perso::set_power(const double power){
    _power = power;
}

void
Perso::set_MR(const double mr){
    _mr = mr;
}

void
Perso::set_luck(const double luck){
    _luck = luck;
}

void
Perso::set_mobility(const int mobility){
    _mob = mobility;
}

void
Perso::set_level(const int level){
    _lvl = level;
}

double
Perso::get_HP(){
    return _HP+_shield.get_HP()+_weapon.get_HP();
}

double
Perso::get_max_HP(){
    return _max_HP;
}

double
Perso::get_MP(){
    return _MP+_shield.get_MP()+_weapon.get_MP();
}

double
Perso::get_max_MP(){
    return _max_MP;
}

double
Perso::get_def(){
    return _def+_shield.get_def()+_weapon.get_def();
}

double
Perso::get_strength(){
    return _strength+_shield.get_strength()+_weapon.get_strength();
}

double
Perso::get_power(){
    return _power+_shield.get_power()+_weapon.get_power();
}

double
Perso::get_MR(){
    return _mr+_shield.get_MR()+_weapon.get_MR();
}

double
Perso::get_luck(){
    return _luck+_shield.get_luck()+_weapon.get_luck();
}

int
Perso::get_mobility(){
    return _mob;
}

int
Perso::get_level(){
    return _lvl;
}

int
Perso::inc_level(){
    _lvl++;
    return _lvl;
}


Item&
Perso::get_shield(){
    return _shield;
}

void
Perso::set_shield(Item& shield){
    //  _shield = shield;
}

Item&
Perso::get_weapon(){
    return _weapon;
}

void
Perso::set_weapon(Item& weapon){
    //  _weapon = weapon;
}

int
Perso::get_XP(){
    return _xp;
}

int
Perso::get_max_XP(){
    return _max_xp;
}

void
Perso::set_XP(int val){
    _xp = val;
}

void
Perso::set_max_XP(int val){
    _max_xp = val;
}

void
Perso::inc_XP(int val){
    _xp += val;
}

void
Perso::inc_max_XP(int val){
    _max_xp += val;
}

std::vector<PState>
Perso::get_states(){
    return _states;
}

void
Perso::add_state(PState state){
    _states.push_back(state);
}

void
Perso::remove_state(PState state){
    std::vector<PState>::iterator it = _states.begin();
    for (unsigned int i = 0;i != _states.size(); ++i, ++it){
        if (_states.at(i) == state){
            _states.erase(it);
        }
    }
}

void
Perso::clear_states(){
    std::vector<PState>::iterator it = _states.begin();
    for (unsigned int i = 0;i != _states.size(); ++i, ++it){
        if (_states.at(i) == BOOST){
            _states.erase(it);
        }
    }
}

bool
Perso::has_moved() const{
    return _has_moved;
}

void Perso::set_moved(bool mv)
{
    _has_moved = mv;
    emit signal_set_has_moved(_has_moved);
}

void
Perso::slot_set_has_moved(){
    set_moved(true);
}

void
Perso::slot_reset_has_moved(){
    set_moved(false);
}
int
Perso::get_player_id() {
    return _player_id;
}


void
Perso::load_caracteristics(){
    // Initializing xml document class
    QDomDocument doc;
    {
        QFile f("Ennemis.xml");
        f.open(QIODevice::ReadOnly);
        doc.setContent(&f);
        f.close();
    }

    // Filling data
    {
        QDomElement root=doc.firstChild().toElement();
        QDomElement child=root.firstChild().toElement();

        while(!child.isNull()) {
            if(child.tagName() == "enemies") {
                QDomElement enemy_node = child.firstChild().toElement();
                while(!enemy_node.isNull()) {
                    // If we are
                    if (enemy_node.tagName() == "enemy" && enemy_node.attribute("id").toStdString()==_name) {
                        QDomElement tag_child = enemy_node.firstChild().toElement();
                        while(!tag_child.isNull()) {
                            if (tag_child.tagName() == "lvl") {
                                if (tag_child.attribute("id").toInt() == _lvl) {
                                    set_HP(tag_child.attribute("HP").toInt());
                                    set_MP(tag_child.attribute("MP").toInt());
                                    set_max_HP(tag_child.attribute("HP").toInt());
                                    set_max_MP(tag_child.attribute("MP").toInt());
                                    set_def(tag_child.attribute("def").toInt());
                                    set_strength(tag_child.attribute("str").toInt());
                                    set_mobility(tag_child.attribute("mob").toInt());
                                }
                            }
                            tag_child = tag_child.nextSibling().toElement();
                        }
                    }
                    enemy_node = enemy_node.nextSibling().toElement();
                }
            }
            child = child.nextSibling().toElement();
        }
    }
}
