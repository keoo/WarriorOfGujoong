#include "Position.hpp"
#include "WGObject.hpp"

WGObject::WGObject():QObject(){
  _name = "";
  _pos = Position(0.0,0.0,0.0);
}
WGObject::WGObject(QObject* obj):QObject(obj){
  _name = "";
  _pos = Position(0.0,0.0,0.0);
}
WGObject::WGObject(WGObject &obj):QObject(obj.parent()){
  _name = obj.get_name();
  _pos  = obj.get_position();
}

WGObject::~WGObject(){
}

void
WGObject::set_name(const std::string &name){
  _name = name;
}

void
WGObject::set_position(const Position& p){
  _pos = p;
}

std::string
WGObject::get_name(){
  return _name;
}

Position&
WGObject::get_position(){
  return _pos;
}

void
WGObject::move(Position &pos){
  _pos.move(pos);
}
