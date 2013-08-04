#ifndef GRAPHICDEADEFFECT_HPP
#define GRAPHICDEADEFFECT_HPP

#include <QGraphicsEffect>
#include <QTimer>

class GraphicDeadEffect : public QGraphicsEffect
{
    Q_OBJECT

    // Timer for the animation
    QTimer _timer;

    int _pix_size_x;
    int _pix_size_y;

    bool **_pixels_cleared;

public:
    GraphicDeadEffect(QObject *parent = 0);
    ~GraphicDeadEffect();

public slots:
    void updateAnimation();

signals:
    void signal_end_of_animation();

protected:
    void draw(QPainter *painter);
};

#endif // GRAPHICDEADEFFECT_HPP
