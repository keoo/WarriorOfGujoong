#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>
#include <QPointer>
#include "modelworld.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    // Sets the map as current map. Does not free the memory used by the old map.
    void create_world(ModelWorld *new_model_world, const QString &world_name);

private:
    // Data of the current map
    ModelWorld *_current_map;

    void create_map(const QSharedPointer<ModelArea> &area);

signals:
    
public slots:
    
};

#endif // GRAPHICSSCENE_HPP
