#include <iostream>
/* -- */
#include <QPixmap>
/* -- */
#include "tiledata.hpp"

TileData::TileData(QPixmap *pixmap, bool walkable) : _pixmap(pixmap), _is_walkable(walkable), _is_walkable_for_action(true)
{
}

TileData::TileData(const TileData &data) : QObject()
{
    _pixmap = data._pixmap;
    _is_walkable = data._is_walkable;
    _is_walkable_for_action = data._is_walkable_for_action;
}

TileData &TileData::operator =(const TileData &data)
{
    _pixmap = data._pixmap;
    _is_walkable = data._is_walkable;
    _is_walkable_for_action = data._is_walkable_for_action;

    return *this;
}

QPixmap &TileData::get_pixmap()
{
    return *_pixmap;
}

bool TileData::is_walkable()
{
    return _is_walkable;
}

void TileData::set_walkable_for_action(bool walk)
{
    _is_walkable_for_action = walk;
    emit signal_walkable_for_action(walk);
}

bool TileData::is_walkable_for_action()
{
    return _is_walkable_for_action;
}
