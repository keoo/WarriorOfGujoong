#include <map>
#include <QString>
#include <QSharedPointer>
#include <QPixmap>
#include <QSize>
#include "modelarea.h"
#include "graphicsscene.hpp"

#include <QGraphicsPixmapItem>
#include <iostream>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), _current_map(NULL)
{
    qDebug("Create scene");
}

void GraphicsScene::create_world(ModelWorld *new_model_world, const QString &world_name)
{
    // Remove previous one
    delete _current_map;

    // Set the new one
    _current_map = new_model_world;

    const std::map<QString, QSharedPointer<ModelArea> > &model_area = _current_map->get_modelarea_map();

    if(model_area.find(world_name) != model_area.end()) {
        qDebug("Map found");
        create_map(model_area.at(world_name));
    }
    else {
        throw ("map " + world_name + " not found");
    }
}

void GraphicsScene::create_map(const QSharedPointer <ModelArea> &area)  {
    const int map_width = area->get_width();
    const int map_height = area->get_height();

    const std::vector < std::vector<QSharedPointer<QPixmap> > > &tiles = area->get_tiles_grid();

    for(int i = 0 ; i < map_width ; ++ i) {
        for(int j = 0 ; j < map_height ; ++ j) {
            const QPixmap *tile = (tiles[i][j]).data();
            std::cout << i << " " << j << " " << tile << " is null : " << tile->isNull() << std::endl;
            QSize size = tile->size();
            // debug purpose
            addRect(i*size.width(), j*size.height(), size.width(), size.height());

            // add the tile to the scene at the good position
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(*tile);
            addItem(item);
            item->moveBy(i*size.width(), j*size.height());
        }
    }
}
