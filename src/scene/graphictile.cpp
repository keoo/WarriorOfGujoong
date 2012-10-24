#include <iostream>
/* -- */
#include <QPixmap>
#include <QGraphicsColorizeEffect>
/* -- */
#include "core/map_data/tiledata.hpp"
/* -- */
#include "graphictile.hpp"

GraphicTile::GraphicTile(TileData *tile) : QObject(), QGraphicsPixmapItem(tile->get_pixmap()), _tile(tile), _effect(new QGraphicsColorizeEffect)
{
    setZValue(0);

    connect(tile, SIGNAL(signal_walkable_for_action(bool)), this, SLOT(set_walkable(bool)));

    // When cases are not walkable
    _effect->setColor(Qt::black);
    _effect->setEnabled(false);
    setGraphicsEffect(_effect);
}

void GraphicTile::set_walkable(bool can_walk)
{
    _effect->setEnabled(!can_walk);
}


TileData *GraphicTile::get_tile()
{
    return _tile;
}
