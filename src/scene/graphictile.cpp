#include "graphictile.hpp"

GraphicTile::GraphicTile(const QPixmap *tile) : QGraphicsPixmapItem(*tile)
{
    setZValue(0);
}

void GraphicTile::set_walkable(bool value)
{
    _is_walkable = value;
    if(!_is_walkable) {
        setOpacity(0.5);
    }
    else {
        setOpacity(1);
    }
}
