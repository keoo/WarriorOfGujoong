#ifndef GRAPHICTILE_HPP
#define GRAPHICTILE_HPP

#include <QGraphicsPixmapItem>

class GraphicTile : public QGraphicsPixmapItem
{
public:
    // FIXME Change  to GraphicTile(const TileData &tile);
    GraphicTile(const QPixmap *tile);

    // You can walk on case where there is people but you can't stop on them
    void set_walkable(bool value);
    void set_stoppable(bool value);

    // TMP to remove after
    bool is_walkable() {return _is_walkable;}

private:
    bool _is_walkable;
};

#endif // GRAPHICTILE_HPP
