#include <QObject>
/* -- */
#include "Perso.hpp"
/* -- */
#include "player.hpp"

Player::Player(int id) : _id(id), _persos(QList<Perso *>())
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
