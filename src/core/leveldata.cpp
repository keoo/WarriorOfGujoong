#include <QString>
#include <QPoint>
/* -- */
#include "core/map_data/modelarea.h"
#include "Perso.hpp"
#include "player.hpp"
#include "leveldata.hpp"

LevelData::LevelData(const QList<Player *> &players, const QString &map_area_id) : QObject()
{
    _players = players;
    _map_id = map_area_id;
    _current_player = 0;
}

LevelData::~LevelData()
{
    foreach(Player *p, _players) {
        delete p;
    }
    _players.clear();
}

const QString &LevelData::get_map_id() const
{
    return _map_id;
}


const QList<Player *> &LevelData::get_players() const
{
    return _players;
}

int LevelData::get_current_player()
{
    return _current_player;
}

void LevelData::set_next_player()
{
    _current_player ++;
    if(_current_player >= _players.size()) {
        _current_player = 0;
    }
}

void LevelData::set_model_area(const QSharedPointer<ModelArea> &model)
{
    _model_area = model;
}

QSharedPointer<ModelArea> &LevelData::get_model_area()
{
    return _model_area;
}

bool LevelData::has_ennemi_around(const QPoint &pos)
{
    foreach(Player *p, _players) {
        if(p != _players[_current_player]) {
            foreach(Perso *perso, p->get_persos()) {
                if(perso->get_position().distance_to(pos) == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}
