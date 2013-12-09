#include <map>
#include <iostream>
/* -- */
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QString>
#include <QSharedPointer>
#include <QPixmap>
#include <QSize>
#include <QVector>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
/* -- translation -- */
#include "constants/ChainConstants.hpp"
/* -- */
#include "core/Perso.hpp"
#include "core/player.hpp"
#include "core/leveldata.hpp"
/* -- */
#include "core/map_data/tiledata.hpp"
#include "core/map_data/modelarea.h"
#include "core/map_data/modelworld.h"
/* -- */
#include "scene/graphicsobject.hpp"
#include "scene/graphictile.hpp"
#include "scene/actionmenuwindow.hpp"
#include "scene/graphicdialog.hpp"
#include "computemoves.hpp"
/* -- */
#include "scene/graphicsscene.hpp"
/* -- */

#define TILE_SIZE 48

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), _current_state(WAITING), _current_map(NULL), _cursor_position(new QGraphicsRectItem()), _selected_item(NULL),
    _attack_item(new QGraphicsPixmapItem(QPixmap(Constants::IMAGES_PATH+"weapons/sword_bronze.png").scaled(TILE_SIZE, TILE_SIZE))),
    _action_menu(new ActionMenuWindow()), _dialogs(NULL)
{
    setBackgroundBrush(QBrush(Qt::black));
}

GraphicsScene::~GraphicsScene() {
    free_data();
    delete _cursor_position;
    delete _action_menu;
}

void GraphicsScene::free_data() {
    _current_state = WAITING;

    _tilesData.clear();

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

void GraphicsScene::create_world(LevelData *mapData)
{
    free_data();

    _current_map = mapData;
    qDebug("Map found");
    create_map(_current_map->get_model_area());

    // Add players object
    foreach(Player *player, _current_map->get_players()) {
        add_objects(player->get_persos());

        connect(player, SIGNAL(signal_player_has_lost(Player *)), this, SLOT(slot_player_has_lost(Player *)));
    }

    // TODO Set to tile size
    _cursor_position->setRect(0, 0, TILE_SIZE, TILE_SIZE);
    addItem(_cursor_position);

    addItem(_attack_item);
    _attack_item->setZValue(110);
    _attack_item->setVisible(false);

    connect(this, SIGNAL(signal_end_of_turn()), _current_map, SLOT(set_next_player()));

    // Show the dialogs first
    _dialogs = new GraphicDialog(this, _current_map->get_dialogs(), DialogPosition(CENTER, BOTTOM));
    connect(_dialogs, SIGNAL(signal_end_of_dialogs()), this, SLOT(hide_dialogs()));
    _dialogs->next_text();
}

void GraphicsScene::add_objects(const QList<Perso *> objects) {
    foreach(Perso *obj, objects) {
        GraphicsObject *graphicObject = new GraphicsObject(obj);

        graphicObject->setPos(obj->get_position().getX()*TILE_SIZE, obj->get_position().getY()*TILE_SIZE);
        graphicObject->setZValue(10);

        addItem(graphicObject);
        _persos.push_back(graphicObject);

        // Signal/slot connections
        // Connect end of turn signal with all persos
        connect(this, SIGNAL(signal_end_of_turn()), obj, SLOT(slot_reset_has_moved()));
        connect(obj, SIGNAL(signal_perso_is_dead(Perso*)), this, SLOT(slot_perso_is_dead(Perso *)));


        connect(_current_map, SIGNAL(signal_change_current_player(int)), graphicObject, SLOT(slot_set_current_player(int)));

        // You have to do it once to initialize data
        graphicObject->slot_set_current_player(_current_map->get_current_player());
    }
}

void GraphicsScene::create_map(const QSharedPointer <ModelArea> &area)  {
    const int map_width = area->get_width();
    const int map_height = area->get_height();

    const std::vector < std::vector<QSharedPointer<TileData> > > &tiles = area->get_tiles_grid();
    for(int i = 0 ; i < map_width ; ++ i) {
        QVector<QSharedPointer<GraphicTile> > lineTiles;
        for(int j = 0 ; j < map_height ; ++ j) {

            TileData *tile = (tiles[i][j]).data();

            const QSize &size = tile->get_pixmap().size();
            // add the tile to the scene at the good position
            QSharedPointer<GraphicTile> graphicTile(new GraphicTile(tile));
            lineTiles.push_back(graphicTile);
            addItem(graphicTile.data());
            graphicTile.data()->moveBy(i*size.width(), j*size.height());
        }
        _tilesData.push_back(lineTiles);
    }
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_dialogs) {
        // Do nothing
    }
    else {
        if(_current_state == WAITING) {
            move_action(event->scenePos());
        }
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_dialogs) {
        _dialogs->mousePressEvent(event);
    }
    else {
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
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{

    if(_dialogs) {
        _dialogs->keyPressEvent(event);
    }
    else {
        switch(_current_state) {
        case WAITING:
            switch(event->key()) {
            case Qt::Key_Escape:
                if(finish_turn()) {
                    // signal connected to slots which change current player and
                    // reinit perso moves
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
                move_action(_cursor_position->pos() + QPointF(1, -TILE_SIZE+1));
                break;
            case Qt::Key_Down:
                move_action(_cursor_position->pos() + QPointF(1, TILE_SIZE+1));
                break;
            case Qt::Key_Right:
                move_action(_cursor_position->pos() + QPointF(TILE_SIZE+1, 1));
                break;
            case Qt::Key_Left:
                move_action(_cursor_position->pos() + QPointF(-TILE_SIZE+1, 1));
                break;
            case Qt::Key_S:
                emit signal_show_stats();
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
                Perso *attacker = _selected_item->get_object();
                Perso *opponent = _current_map->get_perso_at((_attack_item->pos().toPoint()/TILE_SIZE));

                // Then finish the move
                move_finished();
                if(attacker != opponent) { // psychopath are not allowed in this game
                    emit signal_begin_fight(attacker, opponent);
                }

                break;
            }
            break;
        default:
            break;
        }
    }
}

void GraphicsScene::click_action(const QPointF &pos) {
    // If an object is selected, this is the end of the move
    if(has_selected_object()) {
        QPointF point_pos;
        point_pos.setX(((int)pos.x() / TILE_SIZE) * TILE_SIZE);
        point_pos.setY(((int)pos.y() / TILE_SIZE) * TILE_SIZE);

        // If we can not stop on the case, we refuse the event
        const int x = (int)pos.x()/TILE_SIZE;
        const int y = (int)pos.y()/TILE_SIZE;

        if(!_tilesData[x][y].data()->get_tile()->is_walkable_for_action()) {
            return;
        }

        // Finish the move
        connect(_selected_item, SIGNAL(signal_finish_moved()), this, SLOT(propose_end_of_move_action()));
        _selected_item->move_object_to(point_pos);

        // disable using any key or mouse
        _current_state = MOVING;

        // Reset the tiles
        for (int i = 0 ; i < _tilesData.size() ; ++ i) {
            for (int j = 0 ; j < _tilesData[i].size() ; ++ j) {
                _tilesData[i][j].data()->get_tile()->set_walkable_for_action(true);
            }
        }
    }
    else {
        // Look if there is a perso at the position
        foreach(QGraphicsItem *it, items(pos)) {
            GraphicsObject *perso = qgraphicsitem_cast<GraphicsObject *>(it);
            // If the perso has not moved and belongs to the current player, we select it
            if(perso && !perso->has_moved() && perso->get_object()->get_player_id() == _current_map->get_current_player()) {
                // Select the perso
                select_object(qgraphicsitem_cast<GraphicsObject *>(perso));

                // Compute visibility to move
                ComputeMoves::compute_visibility(_current_map->get_model_area(), perso, _persos);
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
    QGraphicsItem *item = itemAt(new_pos, QTransform());

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

    views()[0]->ensureVisible(_cursor_position);
}


void GraphicsScene::move_attack_sword(const QPointF &new_pos) {
    // Get the position in cases

    QPoint map_position(new_pos.toPoint() / TILE_SIZE);
    QPointF cursor_pos(map_position * TILE_SIZE);

    Perso *perso = _current_map->get_perso_at(map_position);

    // Look if there is a perso at the new position
    if(_selected_item->get_object()->get_position().distance_to(map_position) <= 1) {
        if(perso && (perso->get_player_id() != _current_map->get_current_player() || perso == _selected_item->get_object())) {
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

bool GraphicsScene::has_ennemi_perso_around(const QPointF &pt) {
    return _current_map->has_ennemi_around((pt/TILE_SIZE).toPoint());
}

void GraphicsScene::propose_end_of_move_action() {
    // Do action (spells if possible, fight if possible, nothing (always possible))
    QStringList possible_actions;
    if(1/*TODO _selected_item->get_object()->has_spells()*/) {
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
                    move_finished();
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

void GraphicsScene::hide_dialogs()
{
    _dialogs->deleteLater();
    _dialogs = NULL;
}

void GraphicsScene::slot_player_has_lost(Player *p)
{
    // TODO
    std::cout << "player " << p << " has lost" << std::endl;
}

void GraphicsScene::slot_perso_is_dead(Perso *perso)
{
    int id = -1;
    for(int i = 0 ; i < _persos.size() ; ++ i) {
        if(_persos[i]->get_object() == perso)
            id = i;
    }
    if(id == -1) {
        qDebug("error...");
    }
    else {
        _persos.remove(id);
    }
}

void GraphicsScene::move_finished() {
    if(_selected_item) {
        // Finish the perso action
        disconnect(_selected_item, SIGNAL(signal_finish_moved()), this, SLOT(propose_end_of_move_action()));
        unselect_object();
    }
}
