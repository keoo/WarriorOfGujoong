#include <QString>
#include <QPoint>
#include <QFile>
#include <QDomElement>
/* -- */
#include "core/map_data/modelarea.h"
#include "core/map_data/dialogtext.hpp"
#include "Perso.hpp"
#include "player.hpp"
#include "leveldata.hpp"

LevelData::LevelData(QList<Player *> &players, const QString &map_area_id) : QObject()
{
    load_dialogs(map_area_id);
    load_ennemies(map_area_id, players);

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

Perso *LevelData::get_perso_at(const QPoint &pos)
{
    foreach(Player *p, _players) {
        foreach(Perso *perso, p->get_persos()) {
            if(perso->get_position().distance_to(pos) == 0) {
                return perso;
            }
        }
    }
    return NULL;
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

    emit signal_change_current_player(_current_player);
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

const QList<QSharedPointer<DialogText> > &LevelData::get_dialogs() const
{
    return _dialogs;
}

void LevelData::load_dialogs(const QString &map_area_id)
{
    // Initializing xml document class
    QDomDocument doc;
    {
        QFile f("Dialogs.xml");
        f.open(QIODevice::ReadOnly);
        doc.setContent(&f);
        f.close();
    }

    // Filling dialogs
    {
        QDomElement root=doc.firstChild().toElement();
        QDomElement child=root.firstChild().toElement();
        while(!child.isNull()) {
            // If we are
            if (child.tagName() == "area" && child.attribute("id") == map_area_id) {
                QDomElement tag_child = child.firstChild().toElement();
                while(!tag_child.isNull()) {
                    if (tag_child.tagName() == "phrase") {
                        if (tag_child.hasAttribute("perso") && tag_child.hasAttribute("content")) {
                            QSharedPointer<DialogText> dialog(new DialogText(tag_child.attribute("perso"), tag_child.attribute("content")));
                            _dialogs.push_back(dialog);
                        }
                    }
                    tag_child = tag_child.nextSibling().toElement();
                }
            }
            child = child.nextSibling().toElement();
        }
    }
}

void LevelData::load_ennemies(const QString &map_area_id, QList<Player *> &players)
{
    // Initializing xml document class
    QDomDocument doc;
    {
        QFile f("Persos.xml");
        f.open(QIODevice::ReadOnly);
        if(!f.isOpen()) {
            qDebug(("File " + f.fileName() + " not opened").toStdString().c_str());
        }
        doc.setContent(&f);
        f.close();
    }

    // Other players
    QMap<int, QList<Perso *> > persos;

    // Filling ennemi
    {
        QDomElement root=doc.firstChild().toElement();
        QDomElement child=root.firstChild().toElement();

        while(!child.isNull()) {
            // If we are
            if (child.tagName() == "area" && child.attribute("id") == map_area_id) {
                QDomElement tag_child = child.firstChild().toElement();
                while(!tag_child.isNull()) {
                    if (tag_child.tagName() == "perso") {
                        if (tag_child.hasAttribute("lvl") && tag_child.hasAttribute("x")  && tag_child.hasAttribute("y")) {
                            int persoId = tag_child.attribute("player").toInt();
                            Perso *perso = new Perso(tag_child.attribute("id").toStdString(), persoId);

                            perso->set_position(Position(tag_child.attribute("x").toInt(), tag_child.attribute("y").toInt(), 0));
                            perso->set_level(tag_child.attribute("lvl").toInt());
                            perso->load_caracteristics();
                            persos[persoId].push_back(perso);
                        }
                    }
                    tag_child = tag_child.nextSibling().toElement();
                }
            }
            child = child.nextSibling().toElement();
        }
    }

    // Add players to the game
    for(QMap<int, QList<Perso *> >::iterator it = persos.begin() ; it != persos.end() ; ++ it) {
        Player *p = NULL;
        int player_id = it.key();
        foreach(Player *existing_player, players) {
            if(existing_player->get_id() == player_id) {
                p = existing_player;
            }
        }

        if(!p) { // If the player don't exist we create it
            p = new Player(player_id);
        }

        p->add_persos(it.value());

        players.push_back(p);

    }
}

