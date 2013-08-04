
#include <iostream>
/* -- */
#include <QPixmap>
#include <QPainter>
/* -- */
#include "graphicdeadeffect.hpp"

static const int UPDATE_PERIOD = 20;

GraphicDeadEffect::GraphicDeadEffect(QObject *parent) : QGraphicsEffect(parent)
{
    _pix_size_x = 0;
    _pix_size_y = 0;
}

GraphicDeadEffect::~GraphicDeadEffect()
{
    for(int i = 0 ; i < _pix_size_x ; ++ i) {
        delete _pixels_cleared[i];
    }
    delete _pixels_cleared;
}

void GraphicDeadEffect::draw(QPainter *painter)
{
    QPoint offset;
    // Draw pixmap in device coordinates to avoid pixmap scaling;
    const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);

    if(_pix_size_x == 0) {
    // Initialisation
        _pix_size_x = pixmap.size().width();
        _pix_size_y = pixmap.size().height();
        _pixels_cleared = new bool*[_pix_size_x];
        for(int i = 0 ; i < _pix_size_x ; ++ i) {
            _pixels_cleared[i] = new bool(_pix_size_y);
        }
        _timer.start(UPDATE_PERIOD);
        // Connect animation timer
        connect(&_timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    }

    QImage img = pixmap.toImage();
    painter->setWorldTransform(QTransform());

    for(int i = 0 ; i < _pix_size_x ; i ++) {
        for(int j = 0 ; j < _pix_size_y ; j ++) {
            if(_pixels_cleared[i][j]) {
                img.setPixel(i, j, qRgb(0, 0, 0));
            }
        }
    }
    painter->drawImage(offset, img);
}

void GraphicDeadEffect::updateAnimation()
{
    do {
        int rand_x = qrand() % _pix_size_x;
        int rand_y = qrand() % _pix_size_y;
        if(_pixels_cleared[rand_x][rand_y]) {
            break;
        }
    }
    while(1);
}
