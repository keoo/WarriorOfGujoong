#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QVariant>
#include <QList>

class Perso;

class Player : public QVariant
{
public:
    explicit Player();
    ~Player();

    void set_persos(const QList<Perso *> &persos);

    QList<Perso *> &get_persos();

protected:
    QList <Perso *> _persos;

};

#endif // PLAYER_HPP
