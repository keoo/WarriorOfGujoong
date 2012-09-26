#include "computemoves.hpp"

#include "graphicsobject.hpp"

#include <QBrush>
#include <QPen>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include <iostream>

static const int UPDATE_PERIOD = 200;

GraphicsObject::GraphicsObject(WGObject *obj) : QObject(), _obj(obj)
{

    // Get images depending on the name
    QStringList direction_list;
    direction_list << "haut" << "bas" << "gauche" << "droite";
    for(int i = 0 ; i < direction_list.size() ; i ++) {
        // For each direction, we create a list of images
        QSharedPointer<ImageGroup> img(new ImageGroup());
        img.data()->_current_img = 0;
        int tile_id = 0;
        bool file_exists = true;
        while(file_exists) {
            // Check for next file
            const QString filename = QString::fromStdString(obj->getName()) + "_" + direction_list.at(i) + QString("_%1.png").arg(tile_id);

            if(QFile::exists(filename)) {
                // File exists, we load the pixmap
                img.data()->_items.append(new QGraphicsPixmapItem(QPixmap(filename).scaled(16, 16), this));
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

    _status = new QGraphicsEllipseItem(this);

    // Draw ellipsis on the bottom right of the perso
    const int w =  _current_pixmap->pixmap().size().width();
    const int size_ellipse = w/4;
    _status->setRect(w-size_ellipse, w-size_ellipse, size_ellipse, size_ellipse);
    _status->setBrush(QBrush(Qt::green));
    addToGroup(_status);

    // Box drawn when we select the perso
    _selected_item_box = new QGraphicsRectItem(QRectF(0., 0., boundingRect().width(), boundingRect().height()), this);
    _selected_item_box->setPen(QPen(QBrush(QColor(Qt::yellow)), 1));
    _selected_item_box->setVisible(false);

    connect(&_move_timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));

    // TODO Connect slot_perso_has_move to signal from Perso
    //connect(&_perso, SIGNAL(move(bool)), this, SLOT(slot_perso_has_move(bool)));
}

GraphicsObject::~GraphicsObject()
{
}

void GraphicsObject::move_object_to(const QPointF &new_pos)
{
    std::cout << "Mv from (" << pos().x() << ", "<<pos().y()<<") to (" << new_pos.x() << ", "<<new_pos.y()<<")" << std::endl;

    _actions = ComputeMoves::create_moves(pos(), new_pos);

    _move_timer.start(UPDATE_PERIOD);
}

bool GraphicsObject::has_moved() const
{
    // TODO
    // return _perso.has_moved();
    return _has_move;
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

void GraphicsObject::updateAnimation()
{
    static bool zoom = false;
    static const double scaleFactor = 1.15; //How fast we zoom

    if(!zoom) {
        zoom = true;

        scene()->views().at(0)->scale(scaleFactor, scaleFactor);
    }

    scene()->views().at(0)->centerOn(this);
    // Last move
    if(_actions->get_current_move() >= _actions->get_moves()->size()) {
        if(_actions->get_current_move() != 0) {
            setPos(_actions->get_moves()->at(_actions->get_current_move()-1)->pos_final);
        }
        _current_pixmap->setVisible(false);
        _current_pixmap = _pixmaps[BOTTOM]->_items[0];
        _current_pixmap->setVisible(true);

        // Restart all pixmap counters
        QMap<Direction, QSharedPointer<ImageGroup> >::iterator i = _pixmaps.begin();
        while (i != _pixmaps.end()) {
            ImageGroup * val = i.value().data();
            val->_current_img = 0;
            ++i;
        }


        _move_timer.stop();
        ComputeMoves::release_moves(_actions);
        _actions = NULL;

        scene()->views().at(0)->scale(1./scaleFactor, 1./scaleFactor);
        scene()->views().at(0)->centerOn(0, 0);
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
