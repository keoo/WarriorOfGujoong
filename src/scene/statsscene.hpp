#ifndef STATSSCENE_HPP
#define STATSSCENE_HPP

#include <QGraphicsScene>
#include <QList>

class QKeyEvent;
class QGraphicsGridLayout;
class Player;

class StatsScene : public QGraphicsScene
{
    Q_OBJECT
private:
    QList<Player *> _players;
    QList <QGraphicsGridLayout *> _layouts;

    QGraphicsGridLayout *createPlayerLayout(Player *player);

public:
    StatsScene();
    void set_players(QList<Player *> &players);
    void refresh_stats();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void signal_hide_stats();
};

#endif // STATSSCENE_HPP
