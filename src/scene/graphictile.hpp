#ifndef GRAPHICTILE_HPP
#define GRAPHICTILE_HPP

#include <QGraphicsPixmapItem>

class GraphicTile : public QGraphicsPixmapItem
{
public:
    // FIXME Change  to GraphicTile(const TileData &tile);
    GraphicTile(const QPixmap *tile);

    void set_walkable(bool value);

private:
    bool _is_walkable;
};

#endif // GRAPHICTILE_HPP
