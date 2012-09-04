#include "computemoves.hpp"

#include "graphicsobject.hpp"

#include <QBrush>
#include <QPen>
#include <QTimer>
#include <QStyle>
#include <QRadialGradient>
#include <iostream>

static const int UPDATE_PERIOD = 200;

GraphicsObject::GraphicsObject(WGObject *obj, const QPixmap &pixmap) : QObject(), _obj(obj), _pixmap(new QGraphicsPixmapItem(pixmap, this))
{

    addToGroup(_pixmap);
    _status = new QGraphicsEllipseItem(this);

    // Draw ellipsis on the bottom right of the perso
    const int w = pixmap.size().width();
    const int size_ellipse = w/4;
    _status->setRect(w-size_ellipse, w-size_ellipse, size_ellipse, size_ellipse);

    _status->setBrush(QBrush(Qt::green));

    addToGroup(_status);

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

    _has_move = true;
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

#include <QGraphicsScene>
#include <QGraphicsView>
void GraphicsObject::updateAnimation()
{
    static bool zoom = false;
    static const double scaleFactor = 1.15; //How fast we zoom

    if(!zoom) {
        zoom = true;

        scene()->views().at(0)->scale(scaleFactor, scaleFactor);
    }

    scene()->views().at(0)->centerOn(this);

    if(_actions->get_current_move() >= _actions->get_moves()->size()) {
        if(_actions->get_current_move() != 0) {
            setPos(_actions->get_moves()->at(_actions->get_current_move()-1)->pos_final);
        }
        _pixmap->setPixmap(QPixmap(QString::fromStdString("/tmp/WarriorOfGujoong-tiles/princess.png")).scaled(16, 16));

        _move_timer.stop();
        ComputeMoves::release_moves(_actions);
        _actions = NULL;

        scene()->views().at(0)->scale(1./scaleFactor, 1./scaleFactor);
        scene()->views().at(0)->centerOn(0, 0);
        zoom = false;
    }
    else {
        Move *current_mv = _actions->get_moves()->at(_actions->get_current_move());

        if(_current_image_name == "/tmp/WarriorOfGujoong-tiles/princess_droite_1.png") {
            _current_image_name = "/tmp/WarriorOfGujoong-tiles/princess_droite_2.png";
        }
        else {
            _current_image_name = "/tmp/WarriorOfGujoong-tiles/princess_droite_1.png";
        }

        qreal t = .5;
        QPointF pt = (current_mv->pos_final-current_mv->pos_init) * t;
        moveBy(pt.x(), pt.y());
        _actions->get_current_move() += t;

        _pixmap->setPixmap(QPixmap(QString::fromStdString(_current_image_name)).scaled(16, 16));


    }
}
