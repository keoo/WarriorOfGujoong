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
        if(p != _players[_current_player]) {
            foreach(Perso *perso, p->get_persos()) {
                if(perso->get_position().distance_to(pos) == 0) {
                    return perso;
                }
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
        QFile f("Ennemis.xml");
        f.open(QIODevice::ReadOnly);
        doc.setContent(&f);
        f.close();
    }

    // Player 2
    QList<Perso *> persos;
    Player *p2 = new Player(1);

    // Filling ennemi
    {
        QDomElement root=doc.firstChild().toElement();
        QDomElement child=root.firstChild().toElement();

        while(!child.isNull()) {
            // If we are
            if (child.tagName() == "area" && child.attribute("id") == map_area_id) {
                QDomElement tag_child = child.firstChild().toElement();
                while(!tag_child.isNull()) {
                    if (tag_child.tagName() == "enemy") {
                        if (tag_child.hasAttribute("lvl") && tag_child.hasAttribute("x")  && tag_child.hasAttribute("y")) {
                            Perso *perso = new Perso(tag_child.attribute("id").toStdString(), 1);
                            perso->set_position(Position(tag_child.attribute("x").toInt(), tag_child.attribute("y").toInt(), 0));
                            perso->set_level(tag_child.attribute("lvl").toInt());
                            perso->load_caracteristics();
                            persos.push_back(perso);
                        }
                    }
                    tag_child = tag_child.nextSibling().toElement();
                }
            }
            child = child.nextSibling().toElement();
        }
    }

    p2->set_persos(persos);

    players.push_back(p2);
}

