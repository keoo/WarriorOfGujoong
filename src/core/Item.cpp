/**
 * \brief Implementation of the root class for all the items in WG
 * \file Item.cpp
 */

#include "Item.hpp"
#include <string>

Item::Item():WGObject(NULL){
  _name = "";
  _HP = 0.0;
  _MP = 0.0;
  _strength = 0.0;
  _power = 0.0;
  _def = 0.0;
  _mr = 0.0;
  _luck = 0.0;
}

Item::Item(Item &it):WGObject(&it){
  _HP = it.get_HP();
  _MP = it.get_MP();
  _strength = it.get_strength();
  _power = it.get_power();
  _def = it.get_def();
  _mr = it.get_MR();
  _luck = it.get_luck();
}

Item::Item(WGObject &obj):WGObject(&obj){
  _HP = 0.0;
  _MP = 0.0;
  _strength = 0.0;
  _power = 0.0;
  _def = 0.0;
  _mr = 0.0;
  _luck = 0.0;
}

Item::Item(std::string name,
           double HP, double MP,
           double strength, double power,
           double def, double mr,
           double luck,
           QObject* obj):WGObject(obj){
  _name = name;
  _HP = HP;
  _MP = MP;
  _strength = strength;
  _power = power;
  _def = def;
  _mr = mr;
  _luck = luck;
}

Item::~Item(){
}

void
Item::set_HP(const double hp){
  _HP = hp;
}

void
Item::set_MP(const double mp){
  _MP = mp;
}

void
Item::set_def(const double def){
  _def = def;
}

void
Item::set_strength(const double ad){
  _strength = ad;
}

void
Item::set_power(const double power){
  _power = power;
}

void
Item::set_MR(const double mr){
  _mr = mr;
}

void
Item::set_luck(const double luck){
  _luck = luck;
}

double
Item::get_HP(){
  return _HP;
}

double
Item::get_MP(){
  return _MP;
}

double
Item::get_def(){
  return _def;
}

double
Item::get_strength(){
  return _strength;
}

double
Item::get_power(){
  return _power;
}

double
Item::get_MR(){
  return _mr;
}

double
Item::get_luck(){
  return _luck;
}

std::string
Item::get_name(){
  return _name;
}

