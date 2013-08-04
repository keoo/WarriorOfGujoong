#ifndef STATSSCENE_HPP
#define STATSSCENE_HPP

#include <QGraphicsScene>

class QKeyEvent;

class StatsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    StatsScene();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void signal_hide_stats();
};

#endif // STATSSCENE_HPP
