#include <QObject>
/* -- */
#include "Perso.hpp"
/* -- */
#include "player.hpp"

Player::Player(int id) : QObject(), _id(id), _persos(QList<Perso *>())
{
}

Player::~Player() {
    foreach(Perso *p, _persos) {
        delete p;
    }
    _persos.clear();
}

void Player::add_persos(const QList<Perso *> &persos)
{
    _persos += persos;
    foreach(Perso *p, persos) {
        connect(p, SIGNAL(signal_perso_is_dead(Perso *)), this, SLOT(perso_dead(Perso *)));
    }
}

QList<Perso *> &Player::get_persos()
{
    return _persos;
}

int Player::get_id()
{
    return _id;
}

void Player::perso_dead(Perso *perso)
{
    _persos.removeAll(perso);
    if(_persos.isEmpty()) {
        emit signal_player_has_lost(this);
    }
}
