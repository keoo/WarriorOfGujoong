#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>

class Map;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    // Sets the map as current map. Does not free the memory used by the old map.
    void set_map(Map *new_map);

private:
    // Data of the current map
    Map *current_map;

signals:
    
public slots:
    
};

#endif // GRAPHICSSCENE_HPP
