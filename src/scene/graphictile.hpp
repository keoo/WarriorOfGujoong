#ifndef GRAPHICTILE_HPP
#define GRAPHICTILE_HPP

#include <QGraphicsPixmapItem>
#include <QObject>

class TileData;
class QGraphicsColorizeEffect;

class GraphicTile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    GraphicTile(TileData *tile);

    // You can walk on case where there is people but you can't stop on them
    void set_stoppable(bool value);

    TileData *get_tile();
private:

    TileData *_tile;

    QGraphicsColorizeEffect *_effect;

public slots:
    void set_walkable(bool value);
};

#endif // GRAPHICTILE_HPP
