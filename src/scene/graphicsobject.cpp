#include <QBrush>
#include <QPen>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include <iostream>
/* -- */
#include "constants/ChainConstants.hpp"
/* -- */
#include "scene/graphictile.hpp"
#include "scene/graphicdeadeffect.hpp"
/* -- */
#include "computemoves.hpp"
/* -- */
#include "core/Perso.hpp"
/* -- */
#include "scene/graphicsobject.hpp"

static const int UPDATE_PERIOD = 200;

#define TILE_SIZE 48

GraphicsObject::GraphicsObject(Perso *obj) : QObject(), _perso(obj) {
    // Get images depending on the name
    QStringList direction_list;
    direction_list << "haut" << "bas" << "gauche" << "droite";
    for(int i = 0 ; i < direction_list.size() ; ++ i) {
        // For each direction, we create a list of images
        QSharedPointer<ImageAnimation> img(new ImageAnimation());
        img.data()->_current_img = 0;
        int tile_id = 0;
        bool file_exists = true;
        // Load image for this direction while files exist for this direction
        while(file_exists) {
            // Check for next file
            const QString filename = Constants::IMAGES_PERSOS_PATH + QString::fromStdString(obj->get_name()) + "_" + direction_list.at(i) + QString("_%1.png").arg(tile_id);

            if(QFile::exists(filename)) {
                // File exists, we load the pixmap
                img.data()->_items.append(new QGraphicsPixmapItem(QPixmap(filename).scaled(TILE_SIZE, TILE_SIZE), this));
                img.data()->_items.last()->setVisible(false);
            }
            else {
                file_exists = false;
            }
            std::cout << "Looking for " << filename.toStdString() << " : " << file_exists<< std::endl;

            tile_id ++;
        }
        _pixmaps[(Direction)i] = img;
    }

    // Store default pixmap
    _current_pixmap = _pixmaps[BOTTOM]->_items[0];
    _current_pixmap->setVisible(true);
    addToGroup(_current_pixmap);

    // Draw ellipsis on the bottom right of the perso
    _status = new QGraphicsEllipseItem(this);
    const int w =  _current_pixmap->pixmap().size().width();
    const int size_ellipse = w/4;
    _status->setRect(w-size_ellipse, w-size_ellipse, size_ellipse, size_ellipse);
    _status->setBrush(QBrush(Qt::green));
    addToGroup(_status);

    // Box drawn when we select the perso
    _selected_item_box = new QGraphicsRectItem(QRectF(0., 0., boundingRect().width(), boundingRect().height()), this);
    _selected_item_box->setPen(QPen(QBrush(QColor(Qt::yellow)), 1));
    _selected_item_box->setVisible(false);

    // Connect animation timer
    connect(&_move_timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));

    // Connection between the graphical object and the data
    connect(_perso, SIGNAL(signal_set_has_moved(bool)), this, SLOT(slot_perso_has_move(bool)));

    // Connection between the graphical object and the data
    connect(_perso, SIGNAL(signal_perso_is_dead(Perso *)), this, SLOT(slot_perso_dead(Perso *)));

}

GraphicsObject::~GraphicsObject()
{
}

Perso *GraphicsObject::get_object() const {
    return _perso;
}

void GraphicsObject::move_object_to(const QPointF &new_pos)
{
    std::cout << "Mv from (" << pos().x() << ", "<<pos().y()<<") to (" << new_pos.x() << ", "<<new_pos.y()<<")" << std::endl;

    _actions = ComputeMoves::create_moves(pos(), new_pos);

    _move_timer.start(UPDATE_PERIOD);

    _perso->slot_set_has_moved();
    // Set new position to the data
    _perso->set_position(Position(new_pos.x()/TILE_SIZE, new_pos.y()/TILE_SIZE, 0.));

}

void GraphicsObject::slot_perso_dead(Perso *)
{
    // setGraphicsEffect(new GraphicDeadEffect());
    deleteLater();
}

bool GraphicsObject::has_moved() const
{
    return _perso->has_moved();
}

void GraphicsObject::set_selected(bool select)
{
    _selected_item_box->setVisible(select);
}

void GraphicsObject::slot_perso_has_move(bool has_moved)
{
    if(has_moved) {
        _status->setBrush(QBrush(QColor(Qt::red)));
    }
    else {
        _status->setBrush(QBrush(QColor(Qt::green)));
    }
}

void GraphicsObject::slot_set_current_player(int cur_player)
{
    _status->setVisible(cur_player == _perso->get_player_id());
}

void GraphicsObject::updateAnimation()
{
    static bool zoom = false;
    static const double scaleFactor = 1.15; //How fast we zoom

    if(!zoom) {
        zoom = true;
        scene()->views().at(0)->scale(scaleFactor, scaleFactor);
    }

    // Last move
    if(_actions->get_current_move() >= _actions->get_moves()->size()) {
        _current_pixmap->setVisible(false);
        _current_pixmap = _pixmaps[BOTTOM]->_items[0];
        _current_pixmap->setVisible(true);

        // Restart all pixmap counters
        QMap<Direction, QSharedPointer<ImageAnimation> >::iterator i = _pixmaps.begin();
        while (i != _pixmaps.end()) {
            ImageAnimation * val = i.value().data();
            val->_current_img = 0;
            ++i;
        }

        _move_timer.stop();

        emit signal_finish_moved();

        ComputeMoves::release_moves(_actions);
        _actions = NULL;

        scene()->views().at(0)->scale(1./scaleFactor, 1./scaleFactor);
        zoom = false;
    }
    else {
        Move *current_mv = _actions->get_moves()->at(_actions->get_current_move());

        qreal t = .5;
        QPointF pt = (current_mv->pos_final-current_mv->pos_init) * t;
        moveBy(pt.x(), pt.y());
        _actions->get_current_move() += t;

        _current_pixmap->setVisible(false);
        _current_pixmap = _pixmaps[current_mv->sens]->_items[_pixmaps[current_mv->sens]->_current_img];
        _current_pixmap->setVisible(true);

        _pixmaps[current_mv->sens]->_current_img = (_pixmaps[current_mv->sens]->_current_img+1) % _pixmaps[current_mv->sens]->_items.size();

    }
}
