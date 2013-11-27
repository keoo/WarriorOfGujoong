#include <QGraphicsPixmapItem>
#include <QKeyEvent>
/* -- */
#include "constants/ChainConstants.hpp"
/* -- */
#include "core/Perso.hpp"
/* -- */
#include "fightscene.hpp"


struct FightAction
{
    int pos_x;
    int pos_y;
    QString label;
};

static const FightAction TEXT_ACTION_POSITIONS[] = {{0, 200, Constants::ATTACK}, {100, 200, Constants::MAGIC}, {0, 300, Constants::RUN}};

class CursorActionItem : public QGraphicsPixmapItem
{
    static const int SHIFT_X_CURSOR = 20;

    FightActionEnum action;

public:
    CursorActionItem(const QPixmap &pixmap, FightActionEnum newAction) : QGraphicsPixmapItem(pixmap) { setAction(newAction); }
    void setAction(FightActionEnum newAction) { action = newAction; changePosition(); }
    FightActionEnum getAction() const { return action; }

    void changePosition() {setPos(TEXT_ACTION_POSITIONS[action].pos_x - SHIFT_X_CURSOR, TEXT_ACTION_POSITIONS[action].pos_y);}
};

FightScene::FightScene(QObject *parent) :
    QGraphicsScene(parent)
{
    _attacker_perso_icon = new QGraphicsPixmapItem();
    _opponent_perso_icon = new QGraphicsPixmapItem();
    _attacker_perso_icon->setPos(0, 0);
    _opponent_perso_icon->setPos(100, 0);
    addItem(_attacker_perso_icon);
    addItem(_opponent_perso_icon);

    _cursor_icon = new CursorActionItem(QPixmap(Constants::IMAGES_DATA_PATH + "cursor.png"), ATTACK);
    addItem(_cursor_icon);


    _attack_label = new QGraphicsTextItem(TEXT_ACTION_POSITIONS[ATTACK].label);
    _magic_label = new QGraphicsTextItem(TEXT_ACTION_POSITIONS[MAGIC].label);
    _run_label = new QGraphicsTextItem(TEXT_ACTION_POSITIONS[RUN].label);
    _attack_label->setPos(TEXT_ACTION_POSITIONS[ATTACK].pos_x, TEXT_ACTION_POSITIONS[ATTACK].pos_y);
    _magic_label->setPos(TEXT_ACTION_POSITIONS[MAGIC].pos_x, TEXT_ACTION_POSITIONS[MAGIC].pos_y);
    _run_label->setPos(TEXT_ACTION_POSITIONS[RUN].pos_x, TEXT_ACTION_POSITIONS[RUN].pos_y);

    _attack_label->setDefaultTextColor(Qt::blue);
    _magic_label->setDefaultTextColor(Qt::blue);
    _run_label->setDefaultTextColor(Qt::blue);

    addItem(_attack_label);
    addItem(_magic_label);
    addItem(_run_label);


    _attacker_HP_label = new QGraphicsTextItem();
    _opponent_HP_label = new QGraphicsTextItem();

    _attacker_MP_label = new QGraphicsTextItem();
    _opponent_MP_label = new QGraphicsTextItem();


    _attacker_HP_label->setPos(0, -50);
    _opponent_HP_label->setPos(100, -50);

    _attacker_MP_label->setPos(0, -25);
    _opponent_MP_label->setPos(100, -25);

    addItem(_attacker_HP_label);
    addItem(_opponent_HP_label);
    addItem(_attacker_MP_label);
    addItem(_opponent_MP_label);

    connect(&_end_action_timer, SIGNAL(timeout()), this, SIGNAL(signal_end_fight()));
    _end_action_timer.setSingleShot(true);
}

void FightScene::begin_fight(Perso *yours, Perso *opponent)
{
    _occuring_action = false;

    _cursor_icon->setAction(ATTACK);

    // TODO... things :) (beautifully the scene)
    _attacker = yours;
    _opponent = opponent;

    // Set images
    _attacker_perso_icon->setPixmap(QPixmap(Constants::IMAGES_FACES_PATH + QString::fromStdString(yours->get_name()) + ".png"));
    _opponent_perso_icon->setPixmap(QPixmap(Constants::IMAGES_FACES_PATH + QString::fromStdString(opponent->get_name()) + ".png"));

    // Set HPs, MPs
    update_HP_and_MP();
}

void FightScene::do_attack()
{
    int damage = _attacker->get_strength(); // TODO Do something with _opponent->get_shield()
    if(damage > _opponent->get_HP())
        damage = _opponent->get_HP();

    _opponent->set_HP(_opponent->get_HP() - damage);

    update_HP_and_MP();

    // Timer for the animation
    _end_action_timer.start(2000);
}

void FightScene::update_HP_and_MP()
{
    _attacker_HP_label->setPlainText(QString("HP : %1 / %2").arg(_attacker->get_HP()).arg(_attacker->get_max_HP()));
    _opponent_HP_label->setPlainText(QString("HP : %1 / %2").arg(_opponent->get_HP()).arg(_opponent->get_max_HP()));
    _attacker_MP_label->setPlainText(QString("MP : %1 / %2").arg(_attacker->get_MP()).arg(_attacker->get_max_MP()));
    _opponent_MP_label->setPlainText(QString("MP : %1 / %2").arg(_opponent->get_MP()).arg(_opponent->get_max_MP()));
}

void FightScene::keyPressEvent(QKeyEvent *event)
{

    // If during an animation, return !
    if(_occuring_action)
        return;

    FightActionEnum current_action = _cursor_icon->getAction();

    switch(event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
        if(current_action == ATTACK)
            _cursor_icon->setAction(RUN);
        if(current_action == RUN)
            _cursor_icon->setAction(ATTACK);
        break;
    case Qt::Key_Left:
    case Qt::Key_Right:
        if(current_action == ATTACK)
            _cursor_icon->setAction(MAGIC);
        if(current_action == MAGIC)
            _cursor_icon->setAction(ATTACK);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        _occuring_action = true;
        switch(current_action) {
        case ATTACK:
            do_attack();
             break;
        case MAGIC:
            _occuring_action = false; // No magic yet :)
            break;
        case RUN:
            emit signal_end_fight();
            break;
        }
        break;
    }
}
