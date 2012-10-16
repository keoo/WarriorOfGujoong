#include <QObject>
/* -- */
#include "Perso.hpp"
/* -- */
#include "player.hpp"

Player::Player() : _persos(QList<Perso *>())
{
}

Player::~Player() {
    foreach(Perso *p, _persos) {
        delete p;
    }
    _persos.clear();
}

void Player::set_persos(const QList<Perso *> &persos)
{
    _persos = persos;
}

QList<Perso *> &Player::get_persos()
{
    return _persos;
}
