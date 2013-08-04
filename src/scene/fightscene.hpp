#ifndef FIGHTSCENE_HPP
#define FIGHTSCENE_HPP

#include <QGraphicsScene>
#include <QTimer>

class Perso;
class QGraphicsPixmapItem;
class QGraphicsTextItem;

class CursorActionItem;

enum FightActionEnum
{
    ATTACK = 0,
    MAGIC,
    RUN
};

class FightScene : public QGraphicsScene
{
    Q_OBJECT

private:
    CursorActionItem *_cursor_icon;

    QGraphicsPixmapItem *_attacker_perso_icon;
    QGraphicsPixmapItem *_opponent_perso_icon;
    QGraphicsTextItem *_attack_label;
    QGraphicsTextItem *_magic_label;
    QGraphicsTextItem *_run_label;

    QGraphicsTextItem *_attacker_HP_label;
    QGraphicsTextItem *_opponent_HP_label;

    QGraphicsTextItem *_attacker_MP_label;
    QGraphicsTextItem *_opponent_MP_label;

    Perso *_attacker;
    Perso *_opponent;

    bool _occuring_action; // If we have already chosen an action, we disable the key event manager

    QTimer _end_action_timer; // Timer to emit signal_end_fight

    void update_HP_and_MP();

public:
    explicit FightScene(QObject *parent = 0);
    
    void begin_fight(Perso *yours, Perso *opponent);

    void do_attack();

protected:
    // Methods inherited from the scene
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void signal_end_fight();

public slots:
};

#endif // FIGHTSCENE_HPP
