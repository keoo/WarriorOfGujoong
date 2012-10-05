#include <map>
#include <iostream>

#include <QGraphicsPixmapItem>
#include <QString>
#include <QSharedPointer>
#include <QPixmap>
#include <QSize>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "core/Perso.hpp"

#include "modelarea.h"
#include "modelworld.h"

#include "scene/graphicsscene.hpp"
#include "scene/actionmenuwindow.hpp"

#include "scene/graphicsobject.hpp"

/* -- translation -- */
#include "constants/ChainConstants.hpp"

#define TILE_SIZE 16

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), _current_state(WAITING), _current_map(NULL), _cursor_position(new QGraphicsRectItem()), _selected_item(NULL),
    _attack_item(new QGraphicsPixmapItem(QPixmap("/tmp/WarriorOfGujoong-tiles/weapons/sword_bronze.png").scaled(TILE_SIZE, TILE_SIZE))),
    _action_menu(new ActionMenuWindow())
{
}

GraphicsScene::~GraphicsScene() {
    free_data();
    delete _cursor_position;
}

void GraphicsScene::free_data() {
    _current_state = WAITING;
    // Remove previous one
    delete _current_map;
    _current_map = NULL;
    _selected_item = NULL;
    if(_cursor_position->scene() != NULL) {
        removeItem(_cursor_position);
    }
    if(_attack_item->scene() != NULL) {
        removeItem(_attack_item);
    }

    clear();
}

void GraphicsScene::create_world(ModelWorld *new_model_world, const QString &world_name)
{
    free_data();

    // Set the new one
    _current_map = new_model_world;

    const std::map<QString, QSharedPointer<ModelArea> > &model_area = _current_map->get_modelarea_map();

    if(model_area.find(world_name) != model_area.end()) {
        qDebug("Map found");
        create_map(model_area.at(world_name));
        // TODO Set to tile size
        _cursor_position->setRect(0, 0, TILE_SIZE, TILE_SIZE);
        addItem(_cursor_position);

        addItem(_attack_item);
        _attack_item->setZValue(110);
        _attack_item->setVisible(false);

    }
    else {
        throw ("map " + world_name + " not found");
    }
}

// TMP wait for keoo
void GraphicsScene::add_objects(const QVector<Perso *> objects) {
    // TODO Connect end of turn signal with all persos
    foreach(Perso *obj, objects) {
        GraphicsObject *graphicObject = new GraphicsObject(obj);

        graphicObject->setPos(obj->get_position().getX()*TILE_SIZE, obj->get_position().getY()*TILE_SIZE);
        graphicObject->setZValue(10);

        addItem(graphicObject);
    }
}
// End TMP

void GraphicsScene::create_map(const QSharedPointer <ModelArea> &area)  {
    const int map_width = area->get_width();
    const int map_height = area->get_height();

    const std::vector < std::vector<QSharedPointer<QPixmap> > > &tiles = area->get_tiles_grid();

    for(int i = 0 ; i < map_width ; ++ i) {
        for(int j = 0 ; j < map_height ; ++ j) {
            const QPixmap *tile = (tiles[i][j]).data();
            const QSize &size = tile->size();
            // add the tile to the scene at the good position
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(*tile);
            addItem(item);
            item->setZValue(0);
            item->moveBy(i*size.width(), j*size.height());
        }
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_current_state == WAITING) {
        move_action(event->scenePos());
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_current_state == WAITING && event->button() == Qt::LeftButton) {
        click_action(event->scenePos());
    }
    else {
        // TODO Gérer le cas ou on attaque
        _current_state = WAITING;
        _attack_item->setVisible(false);
        move_finished();

    }
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
    switch(_current_state) {
    case WAITING:
        switch(event->key()) {
        case Qt::Key_Escape:
            if(finish_turn()) {
                // TODO Reinit object turns
                // TODO Change current player
                emit signal_end_of_turn();
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            click_action(_cursor_position->pos());
            break;
        case Qt::Key_Space:
            click_action(_cursor_position->pos());
            break;
        case Qt::Key_Up:
            move_action(_cursor_position->pos() + QPointF(0, -TILE_SIZE));
            break;
        case Qt::Key_Down:
            move_action(_cursor_position->pos() + QPointF(0, TILE_SIZE));
            break;
        case Qt::Key_Right:
            move_action(_cursor_position->pos() + QPointF(TILE_SIZE, 0));
            break;
        case Qt::Key_Left:
            move_action(_cursor_position->pos() + QPointF(-TILE_SIZE, 0));
            break;
        }
        break;
    case END_MOVING:
        switch(event->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Enter:
        case Qt::Key_Return:
            _current_state = WAITING;
            break;
        case Qt::Key_Up:
            // TODO
            break;
        case Qt::Key_Down:
            // TODO
            break;
        case Qt::Key_Right:
            // TODO
            break;
        case Qt::Key_Left:
            // TODO
            break;
        }
        break;
    case ATTACKING:
        switch(event->key()) {
        case Qt::Key_Escape:
            _current_state = END_MOVING;
            propose_end_of_move_action();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            std::cout << "Do action : " << _action_menu->get_action().toStdString() << std::endl;
            _current_state = WAITING;
            _attack_item->setVisible(false);
            // TODO Attack if an ennemi is under the sword

            // Then finish the move
            move_finished();
            break;
            /* TODO Attention si 2 ennemis sont a côté et que le deuxieme n'est pas censé être atteignable, ça
            va être le cas avec le code qui suit... */
        case Qt::Key_Up:
            move_attack_sword(_attack_item->pos() + QPointF(0, -TILE_SIZE));
            break;
        case Qt::Key_Down:
            move_attack_sword(_attack_item->pos() + QPointF(0, TILE_SIZE));
            break;
        case Qt::Key_Right:
            move_attack_sword(_attack_item->pos() + QPointF(TILE_SIZE, 0));
            break;
        case Qt::Key_Left:
            move_attack_sword(_attack_item->pos() + QPointF(-TILE_SIZE, 0));
            break;
        }
        break;
    default:
        break;
    }
}

void GraphicsScene::click_action(const QPointF &pos) {
    if(has_selected_object()) {
        QPointF point_pos;
        point_pos.setX(((int)pos.x() / TILE_SIZE) * TILE_SIZE);
        point_pos.setY(((int)pos.y() / TILE_SIZE) * TILE_SIZE);

        // Finish the move
        connect(_selected_item, SIGNAL(signal_finish_moved(bool)), this, SLOT(propose_end_of_move_action()));
        _selected_item->move_object_to(point_pos);

        // disable using any key or mouse
        _current_state = MOVING;

    }
    else {
        // Look if there is a perso at the position
        foreach(QGraphicsItem *it, items(pos)) {
            GraphicsObject *perso = qgraphicsitem_cast<GraphicsObject *>(it);
            if(perso && !perso->has_moved()) {
                // Select the perso
                select_object(qgraphicsitem_cast<GraphicsObject *>(perso));
            }
        }
    }
}

bool GraphicsScene::finish_turn()
{

    QMessageBox::StandardButton ask_for_finish_turn = QMessageBox::question((QWidget*)this->parent(), Constants::TITLE_ASK_END_TURN,
                                                                            Constants::ASK_END_TURN,
                                                                            QMessageBox::Yes | QMessageBox::No);
    return ask_for_finish_turn == QMessageBox::Yes;
}

void GraphicsScene::move_action(const QPointF &new_pos) {

    // Is there an item at the cursor pos ?
    QGraphicsItem *item = itemAt(new_pos);

    // If it is on the map, we move it
    if(item && !qgraphicsitem_cast<QGraphicsRectItem *>(item)) {
        // Get the position in cases
        QPointF cursor_pos;
        cursor_pos.setX(((int)new_pos.x() / TILE_SIZE) * TILE_SIZE);
        cursor_pos.setY(((int)new_pos.y() / TILE_SIZE) * TILE_SIZE);


        // Look if there is a perso at the old position to hide the view
        foreach(QGraphicsItem *it, items(_cursor_position->pos())) {
            GraphicsObject *perso = qgraphicsitem_cast<GraphicsObject *>(it);
            if(perso) {
                emit signal_perso_mouse_quit_hovered();
                break;
            }
        }

        _cursor_position->setPos(cursor_pos);

        // Look if there is a perso at the new position
        foreach(QGraphicsItem *it, items(new_pos)) {
            GraphicsObject *perso = qgraphicsitem_cast<GraphicsObject *>(it);
            if(perso) {
                emit signal_perso_mouse_hovered(perso->get_object());
                break;
            }
        }
    }
}


void GraphicsScene::move_attack_sword(const QPointF &new_pos) {
    // Get the position in cases
    QPointF cursor_pos;
    cursor_pos.setX(((int)new_pos.x() / TILE_SIZE) * TILE_SIZE);
    cursor_pos.setY(((int)new_pos.y() / TILE_SIZE) * TILE_SIZE);

    // Look if there is a perso at the new position
    foreach(QGraphicsItem *it, items(new_pos)) {
        GraphicsObject *perso = qgraphicsitem_cast<GraphicsObject *>(it);
        if(perso /* && it has to be an ennemi */) {
            _attack_item->setPos(cursor_pos);
        }
    }
}



bool GraphicsScene::has_selected_object() const {
    return _selected_item != NULL;
}

void GraphicsScene::unselect_object() {
    if(_selected_item) {
        _selected_item->set_selected(false);
    }
    _selected_item = NULL;
}

void GraphicsScene::select_object(GraphicsObject *item) {
    _selected_item = item;
    if(item) {
        _selected_item->set_selected(true);
    }
}

bool has_ennemi_perso_around(const QPointF &) {
    // TODO
    return true;
}

void GraphicsScene::propose_end_of_move_action() {
    // Do action (spells if possible, fight if possible, nothing (always possible))
    QStringList possible_actions;
    if(0/*TODO _selected_item->get_object()->has_spells()*/) {
        possible_actions.push_back(Constants::MAGIC);
    }
    if(has_ennemi_perso_around(_cursor_position->pos())) {
        possible_actions.push_back(Constants::ATTACK);
    }
    possible_actions.push_back(Constants::FINISH_ACTION);

    _action_menu->set_actions(possible_actions);

    // While action is not set, we wait for user to choose
    while (_current_state != WAITING && _current_state != ATTACKING) {
        // If there is magic, we have to show the dialog box
        if(possible_actions.contains(Constants::MAGIC)) {
            _current_state = END_MOVING;
            if(QDialog::Accepted == _action_menu->exec()) {
                if(_action_menu->get_action() == Constants::FINISH_ACTION) {
                    _current_state = WAITING;
                }
                else {
                    std::cout << "Want to do : " << _action_menu->get_action().toStdString() << std::endl;
                    if(_action_menu->get_action() == Constants::ATTACK) {
                        _attack_item->setVisible(true);
                        _attack_item->setPos(_selected_item->pos());
                        _current_state = ATTACKING;
                    }
                }
            }
            else {
                // Rejected action, we end the perso turn
                _current_state = WAITING;
                move_finished();
            }
        }
        // There is no magic
        // TODO : on fait quoi ? On affiche quand même le menu pour attaquer ou quitter ou on propose juste le curseur pour attaquer ???
        else if (possible_actions.contains(Constants::ATTACK)) {
            _attack_item->setVisible(true);
            _attack_item->setPos(_selected_item->pos());
            _current_state = ATTACKING;
        }
        else {
            _current_state = WAITING;
            move_finished();
        }
    }
}

void GraphicsScene::move_finished() {
    // Finish the perso action
    disconnect(_selected_item, SIGNAL(signal_finish_moved(bool)), this, SLOT(propose_end_of_move_action()));
    unselect_object();
}
