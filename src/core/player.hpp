#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QVariant>
#include <QList>

class Perso;

class Player : public QObject, public QVariant
{
    Q_OBJECT
public:
    explicit Player(int id);
    ~Player();

    void set_persos(const QList<Perso *> &persos);

    QList<Perso *> &get_persos();

    int get_id();

protected:
    int _id;
    QList <Perso *> _persos;

public slots:
    void perso_dead(Perso *perso);

signals:
    void signal_player_has_lost(Player *p);
};

#endif // PLAYER_HPP
