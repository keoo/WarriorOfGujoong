#include "Position.hh"
#include "WGObject.hh"

WGObject::WGObject():QObject(){
  _name = "";
  _pos = Position(0.0,0.0,0.0);
}
WGObject::WGObject(WGObject &obj):QObject(obj.parent()){
  _name = obj.getName();
  _pos  = obj.getPosition();
}

WGObject::~WGObject(){
}

void
WGObject::setName(const std::string &name){
  _name = name;
}

void
WGObject::setPosition(const Position& p){
  _pos = p;
}

std::string
WGObject::getName(){
  return _name;
}

Position&
WGObject::getPosition(){
  return _pos;
}

void
WGObject::move(Position &pos){
  _pos.move(pos);
}
