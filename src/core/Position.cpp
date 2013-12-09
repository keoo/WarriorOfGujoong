#include <QPoint>
#include <cmath>
#include "Position.hpp"


Position::Position(){
  _x = 0.0;
  _y = 0.0;
  _z = 0.0;
}

Position::Position(Position& p){
  _x = p.getX();
  _y = p.getY();
  _z = p.getZ();
}

Position::Position(const double &x, const double& y, const double& z){
  _x = x;
  _y = y;
  _z = z;
}

Position::~Position(){
}

double
Position::getX(){
  return _x;
}

double
Position::getY(){
  return _y;
}

double
Position::getZ(){
  return _z;
}

void
Position::setX(const double& x){
  _x = x;
}

void
Position::setY(const double& y){
  _y = y;
}

void
Position::setZ(const double& z){
  _z = z;
}

void
Position::SetPosition(Position& p){
  _x = p.getX();
  _y = p.getY();
  _z = p.getZ();
}

void
Position::move(Position& p){
  _x += p.getX();
  _y += p.getY();
  _z += p.getZ();
}

int
Position::distance_to(const QPoint &pt){
    return (abs(_x-pt.x())+abs(_y-pt.y()));
}
