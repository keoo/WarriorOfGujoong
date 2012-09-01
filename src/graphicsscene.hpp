#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>
#include <QPointer>
#include <QVector>
#include "core/WGObject.hpp"
#include "modelworld.h"

class GraphicsObject;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    ~GraphicsScene();

    // Sets the map as current map. Does not free the memory used by the old map.
    void create_world(ModelWorld *new_model_world, const QString &world_name);

    // TMP wait for keoo
    void add_objects(const QVector<WGObject *> objects);
    // End TMP


    void select_object(GraphicsObject *item);
    void unselect_object();
    bool has_selected_object() const;

protected:
    // Methods inherited from the scene
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    // Data of the current map
    ModelWorld *_current_map;

    QGraphicsRectItem *_cursor_position;

    GraphicsObject *_selected_item;

    // Creates the graphical items of the map and display them according to their position
    void create_map(const QSharedPointer<ModelArea> &area);

    // Free the data used by the scene (clear the items list, delete the current map)
    void free_data();

    void click_action(const QPointF &pos);

    void move_action(const QPointF &new_pos);

    bool finish_turn();

signals:
    
public slots:
    
};

#endif // GRAPHICSSCENE_HPP
