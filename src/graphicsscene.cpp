#include "graphicsscene.hpp"

#define SCENE_HEIGHT 400
#define SCENE_WIDTH 600
GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    qDebug("Create scene");

    setSceneRect(-SCENE_WIDTH/2, -SCENE_HEIGHT/2, SCENE_WIDTH, SCENE_HEIGHT);
    // setSceneRect(-_current_map/2, -_current_map/2, _current_map, _current_map);

    addRect(-SCENE_WIDTH/2, -SCENE_HEIGHT/2, SCENE_WIDTH, SCENE_HEIGHT);
    // addRect(-_current_map/2, -_current_map/2, _current_map, _current_map);
}
