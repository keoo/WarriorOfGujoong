#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>
#include <QPointer>
#include <QVector>

class ModelWorld;
class ModelArea;
class GraphicsObject;
class ActionMenuWindow;
class Perso;

enum ActionState {
    WAITING,
    MOVING,
    END_MOVING,
    ATTACKING
};

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

    ~GraphicsScene();

    // Sets the map as current map. Does not free the memory used by the old map.
    void create_world(ModelWorld *new_model_world, const QString &world_name);

    // TODO load from files
    void add_objects(const QVector<Perso *> objects);

    void select_object(GraphicsObject *item);
    void unselect_object();
    bool has_selected_object() const;

protected:
    // Methods inherited from the scene
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:

    ActionState _current_state;

    // Data of the current map
    ModelWorld *_current_map;

    QGraphicsRectItem *_cursor_position;

    GraphicsObject *_selected_item;

    QGraphicsPixmapItem *_attack_item;

    ActionMenuWindow *_action_menu;

    // Creates the graphical items of the map and display them according to their position
    void create_map(const QSharedPointer<ModelArea> &area);

    // Free the data used by the scene (clear the items list, delete the current map)
    void free_data();

    void click_action(const QPointF &pos);

    void move_action(const QPointF &new_pos);

    void move_attack_sword(const QPointF &new_pos);

    bool finish_turn();

    void move_finished();

signals:
    void signal_end_of_turn();
    void signal_perso_mouse_hovered(Perso *perso);
    void signal_perso_mouse_quit_hovered();

public slots:
    void propose_end_of_move_action();

};

#endif // GRAPHICSSCENE_HPP
