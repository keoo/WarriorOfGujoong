#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>
#include <QPointer>
#include <QList>
/* -- */
#include "scene/graphictile.hpp"

class LevelData;
class ModelArea;
class GraphicsObject;
class ActionMenuWindow;
class Perso;
class GraphicDialog;

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

    // Sets the map as current map.
    void create_world(LevelData *mapData);

    void select_object(GraphicsObject *item);
    void unselect_object();
    bool has_selected_object() const;

    // Free the data used by the scene (clear the items list, delete the current map)
    void free_data();

protected:
    // Methods inherited from the scene
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:

    ActionState _current_state;

    // Data of the current map
    LevelData *_current_map;

    QVector<QVector<QSharedPointer<GraphicTile> > > _tilesData;

    // Data for persos
    QVector<GraphicsObject *> _persos;

    QGraphicsRectItem *_cursor_position;

    GraphicsObject *_selected_item;

    QGraphicsPixmapItem *_attack_item;

    ActionMenuWindow *_action_menu;

    GraphicDialog *_dialogs;

    // Creates the graphical items of the map and display them according to their position
    void create_map(const QSharedPointer<ModelArea> &area);

    // TODO load from files
    void add_objects(const QList<Perso *> objects);

    void click_action(const QPointF &pos);

    void move_action(const QPointF &new_pos);

    void move_attack_sword(const QPointF &new_pos);

    bool finish_turn();

    void move_finished();

    bool has_ennemi_perso_around(const QPointF &pt);

signals:
    void signal_end_of_turn();
    void signal_perso_mouse_hovered(Perso *perso);
    void signal_perso_mouse_quit_hovered();

public slots:
    void propose_end_of_move_action();
    void hide_dialogs();
};

#endif // GRAPHICSSCENE_HPP
